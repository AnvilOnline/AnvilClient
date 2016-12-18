#include "Utils\Util.hpp"
#include "Engine.hpp"

namespace AnvilEldorado
{
	const auto ApplyResolutionChange = reinterpret_cast<void(__thiscall *)()>(0xA226D0);

	void ResolutionChangeHook()
	{
		// Update the ingame UI's resolution
		ApplyResolutionChange();

		//
		// TODO: Apply new resolution to any subsystems...
		//
	}

	double AspectRatioHook()
	{
		auto *s_GameResolution = reinterpret_cast<int32_t *>(0x19106C0);

		return ((double)s_GameResolution[0] / (double)s_GameResolution[1]);
	}

	__declspec(naked) void FovHook()
	{
		__asm
		{
			// Override the FOV that the memmove before this sets
			mov eax, ds:[0x189D42C]
			mov ds : [0x2301D98], eax
			mov ecx, [edi + 0x18]
			push 0x50CA08
			ret
		}
	}

	bool Engine::ApplyPatches_Graphics()
	{
		using AnvilCommon::Utils::HookFlags;
		using AnvilCommon::Utils::Util;

			// Runs when the game's resolution is changed
		return Util::ApplyHook(0x621303, ResolutionChangeHook, HookFlags::IsCall)
			// Fix aspect ratio not matching resolution
			&& Util::ApplyHook(0x6648C9, AspectRatioHook, HookFlags::IsCall)
			&& Util::ApplyHook(0x216487, AspectRatioHook, HookFlags::IsCall)
			// Disable converting the game's resolution to 16:9
			&& Util::PatchAddress(0x62217D, "\x90\x90", 2)
			&& Util::PatchAddress(0x622183, "\x90\x90\x90\x90\x90\x90", 6)
			// Allow the user to select any resolution that Windows supports in the settings screen
			&& Util::PatchAddress(0x10BF1B, "\x90\x90", 2)
			&& Util::PatchAddress(0x10BF21, "\x90\x90\x90\x90\x90\x90", 6)
			// Prevent FOV from being overridden when the game loads
			&& Util::PatchAddress(0x25FA79, "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 10)
			&& Util::PatchAddress(0x25FA86, "\x90\x90\x90\x90\x90", 5)
			&& Util::ApplyHook(0x10CA02, FovHook);
	}
}