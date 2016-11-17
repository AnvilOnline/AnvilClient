#include "Patches\Patch.hpp"
#include "Engine.hpp"

using namespace AnvilEldorado;

bool Engine::Init()
{
	return AnvilEldorado::Patches::AnvilPatch().Init();
}