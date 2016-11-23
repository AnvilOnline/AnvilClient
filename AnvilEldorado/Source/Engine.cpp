#include <Windows.h>
#include "Blam\Cache\StringIDCache.hpp"
#include "Patches\Patch.hpp"
#include "Engine.hpp"

using namespace AnvilEldorado;

bool Engine::Init()
{
	if (!Blam::Cache::StringIDCache::Instance.Load("maps\\string_ids.dat"))
	{
		MessageBox(NULL, "Failed to load 'maps\\string_ids.dat'!", "", MB_OK);
	}

	if (!AnvilEldorado::Patches::AnvilPatch().Init())
		return false;

	return true;
}