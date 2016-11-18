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

bool Util::PatchAddressInMemory(uint32_t p_Offset, std::string p_HexString, int32_t p_Length)
{
	return PatchAddressInMemory(reinterpret_cast<void*>(p_Offset), p_HexString, p_Length);
}

bool Util::PatchAddressInMemory(void* p_Address, std::string p_HexString, int32_t p_Length)
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

bool Util::PatchAddressInFile(uint32_t p_Offset, std::string p_HexString, int32_t p_Length)
{
	return PatchAddressInFile(reinterpret_cast<void*>(p_Offset), p_HexString, p_Length);
}

bool Util::PatchAddressInFile(void* p_OffsetInFile, std::string p_HexString, int32_t p_Length)
{
	// Ensure that the address is valid
	if (!p_OffsetInFile)
	{
		WriteLog("could not patch, invalid address passed.");
		return false;
	}

	auto s_BaseAddress = reinterpret_cast<uint64_t>(GetModuleHandle(nullptr));
	auto s_Address = s_BaseAddress + reinterpret_cast<uint8_t*>(p_OffsetInFile);

	return PatchAddressInMemory(s_Address, p_HexString, p_Length);
}

void Util::WriteCall(void *p_Address, void* p_NewFunction)
{
	DWORD temp;
	DWORD temp2;
	uint8_t tempJMP[5] = { 0xE8, 0x90, 0x90, 0x90, 0x90 };
	uint32_t JMPSize = ((uint32_t)p_NewFunction - (uint32_t)p_Address - 5);
	if (!VirtualProtect(p_Address, 5, PAGE_READWRITE, &temp))
	{
		std::stringstream ss;
		ss << "Failed to set protection on memory address " << std::hex << p_Address;
		OutputDebugString(ss.str().c_str());
		return;
	}
	memcpy(&tempJMP[1], &JMPSize, 4);
	memcpy(p_Address, tempJMP, 5);
	VirtualProtect(p_Address, 5, temp, &temp2);
}

void Util::WriteJump(void *p_Address, void* p_NewFunction, int p_Flags)
{
	DWORD temp;
	DWORD temp2;
	uint8_t tempJMP[5] = { 0xE9, 0x90, 0x90, 0x90, 0x90 };
	uint8_t tempJE[6] = { 0x0F, 0x84, 0x90, 0x90, 0x90, 0x90 };
	uint32_t patchSize = (p_Flags & HookFlags::IsJmpIfEqual) ? 6 : 5;
	uint32_t JMPSize = ((uint32_t)p_NewFunction - (uint32_t)p_Address - patchSize);
	if (!VirtualProtect(p_Address, patchSize, PAGE_READWRITE, &temp))
	{
		std::stringstream ss;
		ss << "Failed to set protection on memory address " << std::hex << p_Address;
		OutputDebugString(ss.str().c_str());
		return;
	}
	if (p_Flags & HookFlags::IsJmpIfEqual)
	{
		memcpy(&tempJE[2], &JMPSize, 4);
		memcpy(p_Address, tempJE, 6);
	}
	else
	{
		memcpy(&tempJMP[1], &JMPSize, 4);
		memcpy(p_Address, tempJMP, 5);
	}
	VirtualProtect(p_Address, patchSize, temp, &temp2);
}

void Util::ApplyHook(size_t p_Offset, void *p_DestFunc, int p_Flags)
{
	auto p_Address = reinterpret_cast<void *>(p_Offset);

	if (p_Flags & HookFlags::IsCall)
		WriteCall(p_Address, p_DestFunc);
	else
		WriteJump(p_Address, p_DestFunc, p_Flags);
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