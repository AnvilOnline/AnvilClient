#pragma once
#include <string>

namespace AnvilCommon::Utils
{
	class Util
	{
	public:
		static bool HasCommandLine(std::string p_Command);
		static bool PatchAddressInMemory(uint64_t p_Offset, std::string p_HexString, int32_t p_Length);
		static bool PatchAddressInMemory(void* p_Address, std::string p_HexString, int32_t p_Length);
		static bool PatchAddressInFile(uint64_t p_Offset, std::string p_HexString, int32_t p_Length);
		static bool PatchAddressInFile(void* p_Offset, std::string p_HexString, int32_t p_Length);

		static bool ResumeAllThreads();
	};
}