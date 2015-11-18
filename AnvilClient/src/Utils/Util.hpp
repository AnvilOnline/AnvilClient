#pragma once
#include <string>

namespace Anvil
{
	namespace Utils
	{
		class Util
		{
			static int Match(void* p_SrcArray, void* p_DstArray, const char* p_Mask, unsigned int p_Length);

		public:
			static bool PatchAddressInFile(unsigned int p_Address, std::string p_HexString, int p_Length = -1);
			static bool PatchAddressInMemory(unsigned int p_Address, std::string p_HexString, int p_Length = -1);
			static unsigned long FindPattern(void* p_Address, unsigned int p_Length, const char* p_ByteMask, const char* p_Mask);

			static bool GetExecutableInfo(unsigned long& p_EntryPoint, unsigned long& p_ModuleSize);

			static char** CommandLineToArgvA(char* p_CommandLine, int* p_ArgumentCount);

			static bool ResumeAllThreads();
		};
	}
}