#pragma once
#include <string>

namespace Anvil
{
	namespace Utils
	{
		class Util
		{
			static int Match(void* p_SrcArray, void* p_DstArray, const char* p_Mask, uint32_t p_Length);

		public:
			static bool PatchAddressInFile(uint32_t p_Address, std::string p_HexString, int32_t p_Length = -1);
			static bool PatchAddressInMemory(uint32_t p_Address, std::string p_HexString, int32_t p_Length = -1);
			static uint32_t FindPattern(void* p_Address, uint32_t p_Length, const char* p_ByteMask, const char* p_Mask);

			static bool GetExecutableInfo(uint32_t& p_EntryPoint, uint32_t& p_ModuleSize);

			static char** CommandLineToArgvA(char* p_CommandLine, int* p_ArgumentCount);

			static bool ResumeAllThreads();
		};
	}
}