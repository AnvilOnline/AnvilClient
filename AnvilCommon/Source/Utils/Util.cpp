#include "Util.hpp"
#include <Windows.h>
#include <stdio.h>
#include <shellapi.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <sstream>

#include <Utils/Logger.hpp>

#pragma comment(lib, "psapi.lib")

using namespace AnvilCommon::Utils;

bool Util::HasCommandLine(std::string p_Command)
{
	std::wstring s_Command(p_Command.begin(), p_Command.end());

	wchar_t** s_ArgList = nullptr;
	int s_ArgCount = 0;

	s_ArgList = CommandLineToArgvW(GetCommandLineW(), &s_ArgCount);
	if (!s_ArgList)
		return false;

	for (auto i = 0; i < s_ArgCount; ++i)
		if (std::wstring(s_ArgList[i]) == s_Command)
			return true;

	return false;
}

bool Util::PatchAddressInMemory(const size_t p_Offset, const std::string &p_HexString, const int32_t p_Length)
{
	return PatchAddressInMemory(reinterpret_cast<void *>(p_Offset), p_HexString, p_Length);
}

bool Util::PatchAddressInMemory(void *p_Address, const std::string &p_HexString, const int32_t p_Length)
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
	auto s_Ret = VirtualProtect(reinterpret_cast<void *>(p_Address), s_Length, PAGE_EXECUTE_READWRITE, reinterpret_cast<PDWORD>(&s_Protection));
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

bool Util::PatchAddressInFile(const size_t p_Offset, const std::string &p_HexString, const int32_t p_Length)
{
	return PatchAddressInFile(reinterpret_cast<void*>(p_Offset), p_HexString, p_Length);
}

bool Util::PatchAddressInFile(void *p_Address, const std::string &p_HexString, const int32_t p_Length)
{
	// Ensure that the address is valid
	if (!p_Address)
	{
		WriteLog("could not patch, invalid address passed.");
		return false;
	}

	auto s_BaseAddress = reinterpret_cast<size_t>(GetModuleHandle(nullptr));
	auto s_Address = s_BaseAddress + reinterpret_cast<uint8_t*>(p_Address);

	return PatchAddressInMemory(s_Address, p_HexString, p_Length);
}

bool Util::PatchAddress(const size_t p_Offset, const std::string &p_HexString, const int32_t p_Length, bool p_InMemory)
{
	if (p_InMemory)
		return PatchAddressInMemory(p_Offset, p_HexString, p_Length);

	return PatchAddressInFile(p_Offset, p_HexString, p_Length);
}

bool Util::PatchAddress(void *p_Address, const std::string &p_HexString, const int32_t p_Length, bool p_InMemory)
{
	if (p_InMemory)
		return PatchAddressInMemory(p_Address, p_HexString, p_Length);

	return PatchAddressInFile(p_Address, p_HexString, p_Length);
}

bool Util::ResumeAllThreads()
{
	auto s_ThreadSnap = INVALID_HANDLE_VALUE;

	THREADENTRY32 s_Entry;
	ZeroMemory(&s_Entry, sizeof(s_Entry));
	s_Entry.dwSize = sizeof(THREADENTRY32);

	// Snapshot all running threads
	s_ThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (s_ThreadSnap == INVALID_HANDLE_VALUE)
		return false;

	// Get information about the first thread
	if (!Thread32First(s_ThreadSnap, &s_Entry))
	{
		CloseHandle(s_ThreadSnap);

		WriteLog("Could not get the thread snapshot (%x).", GetLastError());
		return false;
	}

	auto s_CurrentProcess = GetCurrentProcessId();
	do
	{
		if (s_Entry.th32OwnerProcessID == s_CurrentProcess)
		{
			auto l_ThreadHandle = OpenThread(THREAD_SUSPEND_RESUME, FALSE, s_Entry.th32ThreadID);
			if (l_ThreadHandle == INVALID_HANDLE_VALUE)
				continue;

			//WriteLog("Resuming thread : %x.", s_Entry.th32ThreadID);

			ResumeThread(l_ThreadHandle);

			CloseHandle(l_ThreadHandle);
		}
	} while (Thread32Next(s_ThreadSnap, &s_Entry));

	CloseHandle(s_ThreadSnap);

	WriteLog("All threads resumed successfully.");
	return true;
}