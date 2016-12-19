#include "Utils\Hook.hpp"
#include "Utils\Patch.hpp"
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
		using AnvilCommon::Utils::Hook;
		using AnvilCommon::Utils::Patch;

			// Runs when the game's resolution is changed
		return Hook(0x621303, ResolutionChangeHook, HookFlags::IsCall).Apply()
			// Fix aspect ratio not matching resolution
			&& Hook(0x6648C9, AspectRatioHook, HookFlags::IsCall).Apply()
			&& Hook(0x216487, AspectRatioHook, HookFlags::IsCall).Apply()
			// Disable converting the game's resolution to 16:9
			&& Patch::NopFill(0x62217D, 2)
			&& Patch::NopFill(0x622183, 6)
			// Allow the user to select any resolution that Windows supports in the settings screen
			&& Patch::NopFill(0x10BF1B, 2)
			&& Patch::NopFill(0x10BF21, 6)
			// Prevent FOV from being overridden when the game loads
			&& Patch::NopFill(0x25FA79, 10)
			&& Patch::NopFill(0x25FA86, 5)
			&& Hook(0x10CA02, FovHook).Apply();
	}
}