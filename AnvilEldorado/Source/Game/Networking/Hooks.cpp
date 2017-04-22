#include "Hooks.hpp"
#include <Interfaces\Client.hpp>
#include <Interfaces\Engine.hpp>
#include <Game\Networking\XnAddr.hpp>

using namespace AnvilEldorado::Game::Networking;

bool Hooks::Init()
{
	auto s_Base = GetClientInterface()->GetEngine()->ExecutableBase();

	// Fix network debug strings having (null) instead of an IP address
	auto s_Address = s_Base + 0x3F6F0;
	HookFunctionOffset(s_Address, GetIPStringFromInAddr);

	// Fix for XnAddrToInAddr to try checking syslink-menu data for XnAddr->InAddr mapping before consulting XNet
	s_Address = s_Base + 0x12D840;
	HookFunctionOffset(s_Address, XnAddrToInAddr);

	return true;
}

DeclareDetouredFunction(Hooks, char*, __cdecl, GetIPStringFromInAddr, XNADDR* p_InAddr)
{
	static char s_IpAddr[64];
	std::memset(s_IpAddr, 0, sizeof(s_IpAddr));

	// TODO: Figure out what in the hell "type" is stupposed to be
	// auto s_Type = *(uint16_t *)((uint8_t *)p_InAddr + 0x12);
	// It looks like it reads abEnet[0] + abEnet[1] as uint16_t
	sprintf_s(s_IpAddr, sizeof(s_IpAddr), "%hd.%hd.%hd.%hd:%hd (%hd)",
		p_InAddr->ina.s_net, p_InAddr->ina.s_host, p_InAddr->ina.s_lh, p_InAddr->ina.s_impno, p_InAddr->wPortOnline, *(uint16_t *)p_InAddr->abEnet);

	return s_IpAddr;
}

DeclareDetouredFunction(Hooks, bool, __cdecl, XnAddrToInAddr, XNADDR *p_XnAddr, XNKID *p_XnKid, void *p_InAddr)
{
	// Fix for XnAddrToInAddr to try checking syslink-menu data for XnAddr->InAddr mapping before consulting XNet

	auto s_MaxMachines = *(uint32_t*)(0x228E6D8);
	auto *s_SyslinkData = (uint8_t*)*(uint32_t*)(0x228E6D8 + 0x4);

	for (uint32_t i = 0; i < s_MaxMachines; s_SyslinkData += 0x164F8, i++)
	{
		auto *s_Entry = s_SyslinkData;
		auto s_EntryStatus = *s_Entry;

		if (s_EntryStatus == 0)
			continue;
		
		auto *s_XnKid = s_Entry + 0x9E;
		auto *s_XnAddr = s_Entry + 0xAE;
		auto *s_IP = s_Entry + 0x170;
		auto *s_Port = s_Entry + 0x174;
	
		if (memcmp(p_XnAddr, s_XnAddr, 0x10) == 0 && memcmp(p_XnKid, s_XnKid, 0x10) == 0)
		{
			// TODO: map p_InAddr struct:
			//   0x0 - 0x10: IP (first 4 bytes for IPv4, 0x10 for IPv6)
			//   0x10 - 0x12: Port number
			//   0x12 - 0x14: IP length (4 for IPv4, 0x10 for IPv6)
	
			memset(p_InAddr, 0, 0x14);
			memcpy(p_InAddr, s_IP, 4);
			memcpy((uint8_t*)p_InAddr + 0x10, s_Port, 2);
	
			*(uint16_t*)((uint8_t*)p_InAddr + 0x12) = 4;
	
			return 1;
		}
	}

	return o_XnAddrToInAddr(p_XnAddr, p_XnKid, p_InAddr);
}