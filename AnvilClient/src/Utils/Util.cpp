#include "Util.hpp"
#include <utils/Logger.hpp>

#include <Windows.h>
#include <Psapi.h>

#pragma comment(lib, "psapi.lib")

using Anvil::Utils::Util;

#ifdef _WIN32
#define ALIGNMENT 4
#elif _WIN64
#define ALIGNMENT 8
#else
#define ALIGNMENT 0
#endif

bool Util::PatchAddressInFile(unsigned int p_OffsetInFile, std::string p_HexString, int p_Length)
{
	// Ensure that the address is valid
	if (!p_OffsetInFile)
	{
		WriteLog("could not patch, invalid address passed.");
		return false;
	}

	auto s_BaseAddress = reinterpret_cast<unsigned long>(GetModuleHandle(nullptr));
	auto s_Address = s_BaseAddress + p_OffsetInFile;

	return PatchAddressInMemory(s_Address, p_HexString, p_Length);
}

bool Util::PatchAddressInMemory(unsigned p_Address, std::string p_HexString, int p_Length)
{
	// Get the length of our patch
	auto s_Length = (p_Length == -1 ? p_HexString.length() : p_Length);
	if (s_Length <= 0)
	{
		WriteLog("invalid patch data.");
		return false;
	}

	// Hold our previous protections
	auto s_Protection = 0;

	// Change the protection so we can write to the executable without crashing
	auto s_Ret = VirtualProtect(reinterpret_cast<void*>(p_Address), s_Length, PAGE_EXECUTE_READWRITE, reinterpret_cast<PDWORD>(&s_Protection));
	if (!s_Ret)
	{
		WriteLog("could not change protection to patch address %p (%x).", p_Address, GetLastError());
		return false;
	}

	WriteLog("changed protection of %x with a length of %x.", p_Address, s_Length);

	// Copy our patch over
	s_Ret = memcpy_s(reinterpret_cast<void*>(p_Address), s_Length, p_HexString.c_str(), s_Length);
	if (s_Ret)
	{
		WriteLog("could not write patch to address %p.", p_Address);
		return false;
	}

	// Change the protections back
	s_Ret = VirtualProtect(reinterpret_cast<void*>(p_Address), s_Length, s_Protection, reinterpret_cast<PDWORD>(&s_Protection));
	if (!s_Ret)
	{
		WriteLog("could not change protection back to default for address %p, be warned.", p_Address);
		return false;
	}

	// Patch succcessfully applied
	return true;
}

unsigned long Util::FindPattern(void* p_Address, unsigned int p_Length, const char* p_ByteMask, const char* p_Mask)
{
	unsigned char* s_Address = nullptr;
	auto s_Buffer = p_Address;

	auto s_MaskLength = strnlen_s(reinterpret_cast<const char*>(p_Mask), 64);
	for (unsigned int i = 0; i < (p_Length - s_MaskLength); ++i)
	{
		auto l_Result = Match((static_cast<unsigned char*>(s_Buffer) + i), (void*)p_ByteMask, p_Mask, s_MaskLength);
		if (l_Result < 0)
		{
			s_Address = static_cast<unsigned char*>(p_Address) + i;
			break;
		}

		i += l_Result;
	}

	return reinterpret_cast<unsigned long>(s_Address);
}

int Util::Match(void* p_SrcArray, void* p_DstArray, const char* p_Mask, unsigned int p_Length)
{
	auto s_NextStart = 0;
	auto s_Start = static_cast<unsigned char*>(p_DstArray)[0];
	for (unsigned int i = 0; i < p_Length; ++i)
	{
		if (p_Mask[i] == '?')
			continue;

		if (static_cast<unsigned char*>(p_SrcArray)[i] == s_Start)
			s_NextStart = i;

		if (static_cast<unsigned char*>(p_SrcArray)[i] != static_cast<unsigned char*>(p_DstArray)[i])
			return s_NextStart;
	}
	return -1;
}

bool Util::GetExecutableInfo(unsigned long& p_EntryPoint, unsigned long& p_ModuleSize)
{
	MODULEINFO s_ModuleInfo;
	memset(&s_ModuleInfo, 0, sizeof(s_ModuleInfo));

	auto s_Ret = GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &s_ModuleInfo, sizeof(s_ModuleInfo));
	if (!s_Ret)
		return false;

	p_EntryPoint = reinterpret_cast<unsigned long>(s_ModuleInfo.lpBaseOfDll);
	p_ModuleSize = s_ModuleInfo.SizeOfImage;

	return true;
}

char** Util::CommandLineToArgvA(char * p_CommandLine, int * p_ArgumentCount)
{
	PCHAR* argv;
	PCHAR  _argv;
	ULONG   len;
	ULONG   argc;
	CHAR   a;
	ULONG   i, j;

	BOOLEAN  in_QM;
	BOOLEAN  in_TEXT;
	BOOLEAN  in_SPACE;

	len = strlen(p_CommandLine);
	i = ((len + 2) / 2)*sizeof(PVOID) + sizeof(PVOID);

	argv = static_cast<PCHAR*>(GlobalAlloc(GMEM_FIXED,
	                                                 i + (len + 2)*sizeof(CHAR)));

	_argv = reinterpret_cast<PCHAR>(reinterpret_cast<PUCHAR>(argv) + i);

	argc = 0;
	argv[argc] = _argv;
	in_QM = FALSE;
	in_TEXT = FALSE;
	in_SPACE = TRUE;
	i = 0;
	j = 0;

	while (a = p_CommandLine[i]) {
		if (in_QM) {
			if (a == '\"') {
				in_QM = FALSE;
			}
			else {
				_argv[j] = a;
				j++;
			}
		}
		else {
			switch (a) {
			case '\"':
				in_QM = TRUE;
				in_TEXT = TRUE;
				if (in_SPACE) {
					argv[argc] = _argv + j;
					argc++;
				}
				in_SPACE = FALSE;
				break;
			case ' ':
			case '\t':
			case '\n':
			case '\r':
				if (in_TEXT) {
					_argv[j] = '\0';
					j++;
				}
				in_TEXT = FALSE;
				in_SPACE = TRUE;
				break;
			default:
				in_TEXT = TRUE;
				if (in_SPACE) {
					argv[argc] = _argv + j;
					argc++;
				}
				_argv[j] = a;
				j++;
				in_SPACE = FALSE;
				break;
			}
		}
		i++;
	}
	_argv[j] = '\0';
	argv[argc] = nullptr;

	(*p_ArgumentCount) = argc;
	return argv;
}
