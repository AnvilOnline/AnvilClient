#pragma once
#include <Interfaces/Initializable.hpp>
#include <Hooks/Hooking.hpp>

namespace AnvilEldorado::Game
{
	class Hooks :
		public AnvilCommon::Initializable
	{
	protected:
		/// <summary>
		/// Halo map verification
		/// </summary>
		DeclareFunction(bool, __cdecl, Game_VerifyMapSignature, void* p_Data);

		/// <summary>
		/// Saber Halo 3 initialization
		/// </summary>
		DeclareFunction(void*, , Game_InitHalo3);

		/// <summary>
		/// Hook for when tags are loaded
		/// </summary>
		DeclareFunction(int, __cdecl, Game_TagsLoaded, char* p_TagType);

		DeclareFunction(void, __fastcall, sub_591290, void* th);

	public:
		virtual bool Init();
	};
}