#pragma once
#include <Utils/Util.hpp>
#include <Utils/Logger.hpp>
#include <MinHook.h>

#define DeclareFunction(returnType, name, callingMethod, ...) \
	typedef returnType (callingMethod* name ## _t)(__VA_ARGS__); \
	static name ## _t o_ ## name; \
	static returnType callingMethod c_ ## name(__VA_ARGS__);

#define DeclareThiscallFunction(returnType, name, thisType, ...) \
	typedef returnType (__thiscall* name ## _t)(thisType This, __VA_ARGS__); \
	static name ## _t o_ ## name; \
	static returnType __fastcall c_ ## name(thisType, void*, __VA_ARGS__);

#define HookedFunction(nameSpace, returnType, name, callingMethod, ...) \
	returnType callingMethod nameSpace::c_ ## name(__VA_ARGS__)

//#define DeclareHookAtOffset(name, offset) \
//	uint32_t s_ ## name = offset; \
//	o_ ## name = (name ## _t)MH_CreateHook((PBYTE)s_ ## name, (PBYTE)c_ ## name);

#define DeclareHookAtOffset64(name, offset) \
	auto s_ ## name = reinterpret_cast<void*>(offset); \
	if (MH_CreateHook(s_ ## name, reinterpret_cast<void*>(c_ ## name), reinterpret_cast<void**>(&o_ ## name)) != MH_OK) \
	{	WriteLog(L"Hooking failed for %s.", #name); } else { WriteLog(L"%s has been detoured.", #name); }

//#define DeclareHookPattern(name, offset, length, pattern, mask) \
//	uint32_t s_ ## name = Anvil::Utils::Util::FindPattern(reinterpret_cast<void*>(offset), length, pattern, mask); \
//	if (!s_ ## name) { \
//		WriteLog(L"Hooking Failed for %s.", #name); }\
//				else { \
//		o_ ## name = (name ## _t)DetourFunction((PBYTE)s_ ## name, (PBYTE)c_ ## name); \
//		WriteLog(L"%s Hooked.", #name); }

#define DeclareFunctionValue(nameSpace, name) \
	nameSpace ## :: ## name ## _t nameSpace ## :: ## o_ ## name = nullptr;

#define DeclareFunctionValueOffset(nameSpace, name, offset) \
	nameSpace ## :: ## name ## _t nameSpace ## :: ## o_ ## name = offset;