#pragma once
#include <string>

namespace AnvilCommon::Utils
{
	enum HookFlags : int32_t
	{
		None = 0,
		IsCall = 1,
		IsJmpIfEqual = 2
	};

	class Util
	{
	public:
		static bool HasCommandLine(std::string p_Command);

		static bool PatchAddressInMemory(uint64_t p_Offset, std::string p_HexString, int32_t p_Length);
		static bool PatchAddressInMemory(void* p_Address, std::string p_HexString, int32_t p_Length);
		static bool PatchAddressInFile(uint64_t p_Offset, std::string p_HexString, int32_t p_Length);
		static bool PatchAddressInFile(void* p_Offset, std::string p_HexString, int32_t p_Length);
		
		static void WriteCall(void *p_Address, void *p_NewFunction);
		static void WriteJump(void *p_Address, void *p_NewFunction, int p_Flags);
		static void ApplyHook(size_t p_Offset, void *p_DestFunc, int p_Flags);

		static bool ResumeAllThreads();
	};
}