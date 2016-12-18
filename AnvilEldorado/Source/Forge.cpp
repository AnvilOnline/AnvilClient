#include "Utils\Util.hpp"
#include "Blam\Objects\ObjectData.hpp"
#include "Engine.hpp"

namespace AnvilEldorado
{
	bool g_Forge_ShouldDelete = false;

	bool g_Forge_BarriersEnabledValid = false;
	bool g_Forge_KillBarriersEnabled = true;
	bool g_Forge_PushBarriersEnabled = true;

	void UpdateBarriersEnabled()
	{
		// Don't scan multiple times per tick
		if (g_Forge_BarriersEnabledValid)
			return;

		// Scan the object table to check if the barrier disablers are spawned
		auto *s_ObjectArray = Blam::Objects::ObjectData::GetDataArray();

		if (!s_ObjectArray)
			return;

		g_Forge_KillBarriersEnabled = true;
		g_Forge_PushBarriersEnabled = true;

		for (auto &s_Object : *s_ObjectArray)
		{
			// The objects are identified by tag index.
			// scen 0x5728 disables kill barriers
			// scen 0x5729 disables push barriers
			if (s_Object.Type != Blam::Objects::ObjectType::Scenery)
				continue;

			auto s_TagIndex = s_Object.GetTagIndex().Index;

			if (s_TagIndex == 0x5728)
				g_Forge_KillBarriersEnabled = false;
			else if (s_TagIndex == 0x5729)
				g_Forge_PushBarriersEnabled = false;

			if (!g_Forge_KillBarriersEnabled && !g_Forge_PushBarriersEnabled)
				break;
		}

		g_Forge_BarriersEnabledValid = true;
	}

	bool CheckKillTriggersHook(int32_t a0, void *a1)
	{
		UpdateBarriersEnabled();
		if (!g_Forge_KillBarriersEnabled)
			return false;

		typedef bool(*CheckKillTriggersPtr)(int32_t a0, void *a1);
		auto CheckKillTriggers = reinterpret_cast<CheckKillTriggersPtr>(0x68C410);
		return CheckKillTriggers(a0, a1);
	}

	bool ObjectSafeZoneHook(void *a0)
	{
		UpdateBarriersEnabled();
		if (!g_Forge_KillBarriersEnabled)
			return true;

		typedef bool(*CheckSafeZonesPtr)(void *a0);
		auto CheckSafeZones = reinterpret_cast<CheckSafeZonesPtr>(0x4EB130);
		return CheckSafeZones(a0);
	}

	void *PushBarriersGetStructureDesignHook(int32_t index)
	{
		UpdateBarriersEnabled();
		if (!g_Forge_PushBarriersEnabled)
			return nullptr; // Return a null sddt if push barriers are disabled

		typedef void*(*GetStructureDesignPtr)(int32_t index);
		auto GetStructureDesign = reinterpret_cast<GetStructureDesignPtr>(0x4E97D0);
		return GetStructureDesign(index);
	}

	__declspec(naked) void Forge_UpdateInput_Hook()
	{
		__asm
		{
			mov al, g_Forge_ShouldDelete
			test al, al
			jnz del

			// Not deleting - just call the original function
			push esi
			mov eax, 0x59F0E0
			call eax
			retn 4

		del:
			mov g_Forge_ShouldDelete, 0

			// Simulate a Y button press
			mov eax, 0x244D1F0              // Controller data
			mov byte ptr[eax + 0x9E], 1    // Ticks = 1
			and byte ptr[eax + 0x9F], 0xFE // Clear the "handled" flag

											// Call the original function
			push esi
			mov eax, 0x59F0E0
			call eax

			// Make sure nothing else gets the fake press
			mov eax, 0x244D1F0          // Controller data
			or byte ptr[eax + 0x9F], 1 // Set the "handled" flag
			retn 4
		}
	}

	bool Engine::ApplyPatches_Forge()
	{
		using AnvilCommon::Utils::HookFlags;
		using AnvilCommon::Utils::Util;

		// Change the way that Forge handles dpad up so that it doesn't mess with key repeat
		// Comparing the action tick count to 1 instead of using the "handled" flag does roughly the same thing and lets the menu UI read the key too
		Util::PatchAddress(0x19F17F, "\x75", 1);
		Util::PatchAddress(0x19F198, "\x90\x90\x90\x90", 4);

		Util::ApplyHook(0x19D482, Forge_UpdateInput_Hook, HookFlags::IsCall);

		Util::ApplyHook(0x771C7D, CheckKillTriggersHook, HookFlags::IsCall);
		Util::ApplyHook(0x7B4C32, CheckKillTriggersHook, HookFlags::IsCall);

		Util::ApplyHook(0x19EBA1, ObjectSafeZoneHook, HookFlags::IsCall);
		Util::ApplyHook(0x19FDBE, ObjectSafeZoneHook, HookFlags::IsCall);
		Util::ApplyHook(0x19FEEC, ObjectSafeZoneHook, HookFlags::IsCall);
		Util::ApplyHook(0x32663D, ObjectSafeZoneHook, HookFlags::IsCall);

		Util::ApplyHook(0x2749D1, PushBarriersGetStructureDesignHook, HookFlags::IsCall);
		Util::ApplyHook(0x274DBA, PushBarriersGetStructureDesignHook, HookFlags::IsCall);
		Util::ApplyHook(0x2750F8, PushBarriersGetStructureDesignHook, HookFlags::IsCall);
		Util::ApplyHook(0x275655, PushBarriersGetStructureDesignHook, HookFlags::IsCall);

		return true;
	}
}