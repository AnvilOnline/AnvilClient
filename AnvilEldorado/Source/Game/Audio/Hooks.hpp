#pragma once
#include <Interfaces\Initializable.hpp>
#include <Hooking\Hooking.hpp>

namespace AnvilEldorado::Game::Audio
{
	class Hooks :
		public AnvilCommon::Initializable
	{
	protected:
		/// <summary>
		/// VTable Function from vtbl .rdata:01750794 ; const FMOD::EventSystemI::`vftable'
		/// .text:01369AA0 FMOD__System__init
		/// </summary>
		DeclareFunction(void*, __stdcall, FMOD_System_Init, void*, int maxChannels, void*, void*, void*);

		/// <summary>
		/// .text:01388880 FMOD_System_SetSoftwareChannels
		/// </summary>
		DeclareFunction(void*, __stdcall, FMOD_System_SetSoftwareChannels, void*, int);
		
	public:
		virtual bool Init();
	};
}