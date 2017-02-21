#pragma once

#define DeclareFunction(returnType, callingMethod, name, ...) \
	typedef returnType (callingMethod* name ## _t)(__VA_ARGS__); \
	static name ## _t o_ ## name; \
	static returnType callingMethod hk_ ## name(__VA_ARGS__);

#define HookFunctionOffset(offset, name) \
	if (MH_CreateHook(offset, &hk_ ## name, reinterpret_cast<void**>(&o_ ## name)) != MH_OK) \
	{ WriteLog("Could not hook %s.", #name); } else	{ WriteLog("%s hooked.", #name); }