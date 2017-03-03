#pragma once
#include <Interfaces\Initializable.hpp>
#include <Hooks\Hooking.hpp>

namespace AnvilEldorado::Game::Forge
{
	class Hooks :
		public AnvilCommon::Initializable
	{
	protected:
		DeclareFunction(int8_t, __stdcall, Forge_UpdateInput, void* a1);
		DeclareFunction(bool, __cdecl, UpdateKillTriggers, void* a1, void* a2);
		DeclareFunction(bool, __cdecl, CheckSafeZones, void* a1);
		DeclareFunction(void*, __cdecl, PushBarriersGetStructureDesign, void* a1);

	public:
		virtual bool Init();
	};
}