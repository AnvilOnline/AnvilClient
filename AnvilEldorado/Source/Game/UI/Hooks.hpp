#pragma once
#include <Interfaces\Initializable.hpp>

#include <Blam\Text\StringID.hpp>

#include <Hooking\Hooking.hpp>

namespace AnvilEldorado::Game::UI
{
	class Hooks :
		public AnvilCommon::Initializable
	{
	protected:
		DeclareFunction(bool, __cdecl, LocalizeString, int p_TagIndex, Blam::Text::StringID p_StringId, wchar_t* p_Output);

	public:
		virtual bool Init();
	};
}