#include "Patch.hpp"
#include "Hooks\Hook.hpp"
#include "Utils\Util.hpp"

using namespace AnvilCommon;
using namespace AnvilEldorado::Patches;

#include <Windows.h>

void ApplyAfterTagsLoaded()
{
	MessageBox(NULL, "Tags were loaded!", "Tags", MB_OK);

	AnvilEldorado::Hooks::Armor_AfterTagsLoaded();
	MessageBox(NULL, "Armor tags were loaded!", "Tags", MB_OK);

	AnvilEldorado::Hooks::Armor_RefreshUiPlayer();
}

__declspec(naked) void TagsLoadedHook()
{
	__asm
	{
		call ApplyAfterTagsLoaded
		push 0x6D617467
		push 0x5030EF
		ret
	}
}

void AnvilPatch::Patch_Tags()
{
	// Enable Tag Editing
	Utils::Util::PatchAddressInFile(0x101A5B, "\xEB", 1);
	Utils::Util::PatchAddressInFile(0x102874, "\x90\x90", 2);
	Utils::Util::PatchAddressInFile(0x1030AA, "\x90\x90", 2);

	// Used to call Patches::ApplyAfterTagsLoaded when tags have loaded
	Utils::Util::ApplyHook(0x1030EA, TagsLoadedHook);
}
