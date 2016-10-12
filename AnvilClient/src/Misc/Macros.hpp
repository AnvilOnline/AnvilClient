#pragma once
#include <Utils/Util.hpp>
#include <Utils/Logger.hpp>
#include <MinHook.h>

#define DeclareFunction(returnType, name, callingMethod, ...) \
	typedef returnType (callingMethod* name ## _t)(__VA_ARGS__); \
	static name ## _t o_ ## name; \
	static returnType callingMethod c_ ## name(__VA_ARGS__);

#define HookedFunction(nameSpace, returnType, name, callingMethod, ...) \
	nameSpace:: ## name ## _t nameSpace:: ## name = nullptr; \
	returnType callingMethod nameSpace::c_ ## name(__VA_ARGS__)

#define DeclareHookAtOffset64(name, offset) \
	auto s_ ## name = reinterpret_cast<void*>(offset); \
	if (MH_CreateHook(s_ ## name, reinterpret_cast<void*>(c_ ## name), reinterpret_cast<void**>(&o_ ## name)) != MH_OK) \
	{	WriteLog(L"Hooking failed for %s.", #name); } else { WriteLog(L"%s has been detoured.", #name); }
