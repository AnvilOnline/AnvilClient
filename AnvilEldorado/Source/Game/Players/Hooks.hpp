#pragma once
#include <Interfaces\Initializable.hpp>

#include <Hooking\Hooking.hpp>

namespace AnvilEldorado::Game::Players
{
	class Hooks :
		public AnvilCommon::Initializable
	{
	protected:

		DeclareFunction(void, __cdecl, SetupPlayerArmor, double a1, __m128 a2);

	public:
		virtual bool Init();
	};
}