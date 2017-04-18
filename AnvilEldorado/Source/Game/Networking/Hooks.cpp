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
	sprintf_s(s_IpAddr, sizeof(s_IpAddr), "%hd.%hd.%hd.%hd:%hd (%hd)", p_InAddr->ina.s_net,
		p_InAddr->ina.s_host, p_InAddr->ina.s_lh, p_InAddr->ina.s_impno, p_InAddr->wPortOnline, 0);

	return s_IpAddr;
}

DeclareDetouredFunction(Hooks, bool, __cdecl, XnAddrToInAddr, XNADDR* p_XnAddr, XNKID* p_XnKid, IN_ADDR* p_InAddr)
{
	// TODO: What in wizardy magic was in this function???
	// Why tf are we even hooking here

	//		//printf("XnAddrToInAddrHook...");
	//		uint32_t maxMachines = *(uint32_t*)(0x228E6D8);
	//		uint8_t *syslinkDataPtr = (uint8_t*)*(uint32_t*)(0x228E6D8 + 0x4);
	//
	//		for (uint32_t i = 0; i < maxMachines; i++)
	//		{
	//			uint8_t *entryPtr = syslinkDataPtr;
	//			syslinkDataPtr += 0x164F8;
	//			uint8_t entryStatus = *entryPtr;
	//			if (entryStatus == 0)
	//				continue;
	//
	//			uint8_t *xnkidPtr = entryPtr + 0x9E;
	//			uint8_t *xnaddrPtr = entryPtr + 0xAE;
	//			uint8_t *ipPtr = entryPtr + 0x170;
	//			uint8_t *portPtr = entryPtr + 0x174;
	//
	//			if (memcmp(pxna, xnaddrPtr, 0x10) == 0 && memcmp(pxnkid, xnkidPtr, 0x10) == 0)
	//			{
	//				// in_addr struct:
	//				// 0x0 - 0x10: IP (first 4 bytes for IPv4, 0x10 for IPv6)
	//				// 0x10 - 0x12: Port number
	//				// 0x12 - 0x14: IP length (4 for IPv4, 0x10 for IPv6)
	//
	//				memset(in_addr, 0, 0x14);
	//				memcpy(in_addr, ipPtr, 4);
	//				memcpy((uint8_t*)in_addr + 0x10, portPtr, 2);
	//
	//				*(uint16_t*)((uint8_t*)in_addr + 0x12) = 4;
	//
	//				return 1;
	//			}
	//		}

	return o_XnAddrToInAddr(p_XnAddr, p_XnKid, p_InAddr);
}