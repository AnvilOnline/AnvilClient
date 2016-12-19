#include <string>
#include <sstream>
#include "Utils\Hook.hpp"
#include "Utils\Logger.hpp"
#include "ModuleDebug.hpp"

namespace AnvilEldorado::Modules
{
	void* Debug_MemcpyHook(void* dst, void* src, uint32_t len)
	{
		auto srcFilter = ModuleDebug::Instance()->VarMemcpySrc->ValueInt;
		auto dstFilter = ModuleDebug::Instance()->VarMemcpyDst->ValueInt;
		uint32_t srcAddress = (uint32_t)src;
		uint32_t dstAddress = (uint32_t)dst;

		if (srcFilter >= srcAddress && srcFilter < srcAddress + len)
		{

			WriteLog("Memcpy Filter Triggered - Source Address: 0x%08X, Destination Address: 0x%08X, Size: 0x%08X", srcAddress, dstAddress, len);

			// TODO: proper reset
			ModuleDebug::Instance()->VarMemcpySrc->ValueInt = 0;
		}

		if (dstFilter >= dstAddress && dstFilter < dstAddress + len)
		{
			WriteLog("Memcpy Filter Triggered - Source Address: 0x%08X, Destination Address: 0x%08X, Size: 0x%08X", srcAddress, dstAddress, len);

			// TODO: proper reset
			ModuleDebug::Instance()->VarMemcpyDst->ValueInt = 0;
		}

		return memcpy(dst, src, len);
	}

	bool MemcpySrcFilterUpdate(const std::vector<std::string>& Arguments, std::string& returnInfo)
	{
		auto src = ModuleDebug::Instance()->VarMemcpySrc->ValueInt;

		std::stringstream ss;
		ss << "Memcpy source address filter set to " << src;
		returnInfo = ss.str();

		return true;
	}

	bool MemcpyDstFilterUpdate(const std::vector<std::string>& Arguments, std::string& returnInfo)
	{
		auto dst = ModuleDebug::Instance()->VarMemcpyDst->ValueInt;

		std::stringstream ss;
		ss << "Memcpy destination address filter set to " << dst;
		returnInfo = ss.str();

		return true;
	}

	void* Debug_MemsetHook(void* dst, int val, uint32_t len)
	{
		uint32_t dstFilter = ModuleDebug::Instance()->VarMemsetDst->ValueInt;
		uint32_t dstAddress = (uint32_t)dst;

		if (dstFilter >= dstAddress && dstFilter < dstAddress + len)
		{
			WriteLog("Memset Filter Triggered - Destination Address: 0x%08X, Size: 0x%08X", dstAddress, len);

			// TODO: proper reset
			ModuleDebug::Instance()->VarMemsetDst->ValueInt = 0;
		}

		return memset(dst, val, len);
	}

	bool MemsetDstFilterUpdate(const std::vector<std::string>& Arguments, std::string& returnInfo)
	{
		auto dst = ModuleDebug::Instance()->VarMemsetDst->ValueInt;

		std::stringstream ss;
		ss << "Memset destination address filter set to " << dst;
		returnInfo = ss.str();

		return true;
	}

	ModuleDebug::ModuleDebug() : ModuleBase("Debug")
	{
		using AnvilCommon::Utils::Hook;

		VarMemcpySrc = AddVariableInt("MemcpySrc", "memcpy_src", "Allows breakpointing memcpy based on specified source address filter.", eCommandFlagsHidden, 0, MemcpySrcFilterUpdate);
		VarMemcpyDst = AddVariableInt("MemcpyDst", "memcpy_dst", "Allows breakpointing memcpy based on specified destination address filter.", eCommandFlagsHidden, 0, MemcpyDstFilterUpdate);
		VarMemsetDst = AddVariableInt("MemsetDst", "memset_dst", "Allows breakpointing memset based on specified destination address filter.", eCommandFlagsHidden, 0, MemsetDstFilterUpdate);

		Hook(0x7EF260, Debug_MemcpyHook).Apply();
		Hook(0x7EF2E0, Debug_MemsetHook).Apply();
	}
}