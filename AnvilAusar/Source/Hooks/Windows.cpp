#include "Windows.hpp"
#include <Windows.h>
#include <Utils/Logger.hpp>
#include <MinHook.h>

using namespace AnvilAusar::Hooks;

bool WindowsHooks::Init()
{
	auto s_KernelBaseHandle = GetModuleHandle("KernelBase.dll");
	if (!s_KernelBaseHandle)
	{
		WriteLog("Could not get the handle of kernelbase.");
		return false;
	}

	auto s_CreateFileW = GetProcAddress(s_KernelBaseHandle, "CreateFileW");
	
	return false;
}

/*
	==============================================================================
	Hooked Functions
	==============================================================================
*/

void* WindowsHooks::c_CreateFileW(const wchar_t * p_FileName, uint32_t p_Access, void * p_SecurityAttributes, uint32_t p_Creation, uint32_t p_Attributes, void * p_Template)
{
	auto s_Ret = o_CreateFileW(p_FileName, p_Access, p_SecurityAttributes, p_Creation, p_Attributes, p_Template);

	WriteLog("[File Access] %S %x.", p_FileName, s_Ret);

	return s_Ret;
}