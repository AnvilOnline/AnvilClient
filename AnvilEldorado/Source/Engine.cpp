#include "Patches\Patch.hpp"
#include "Engine.hpp"

using namespace AnvilEldorado;

bool Engine::Init()
{
	if (!AnvilEldorado::Patches::AnvilPatch().Init())
		return false;

	return true;
}