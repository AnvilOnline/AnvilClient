#include "Utils\Util.hpp"
#include "Blam\Data\DatumIndex.hpp"
#include "Blam\Game\Players.hpp"
#include "Blam\Objects\ObjectData.hpp"
#include "Blam\Tags\TagInstance.hpp"
#include "Blam\Tags\Items\Weapon.hpp"
#include "Blam\Tags\Scenario\Scenario.hpp"
#include "Engine.hpp"

namespace AnvilEldorado
{
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

	int32_t __cdecl DualWieldHook(const uint16_t p_ObjectIndex)
	{
		using Blam::Data::DatumIndex;
		using Blam::Objects::ObjectData;
		using Blam::Tags::TagInstance;
		using Blam::Tags::Items::Weapon;

		auto s_TagIndex = (*ObjectData::GetDataArray())[p_ObjectIndex].GetTagIndex();
		auto *s_TagDefinition = TagInstance(s_TagIndex).GetDefinition<Weapon>();

		return ((int32_t)s_TagDefinition->WeaponFlags1 & (int32_t)Weapon::Flags1::CanBeDualWielded) != 0;
	}

	int32_t __cdecl GetEquipmentCountHook(const uint16_t p_PlayerObjectIndex, const uint16_t p_EquipmentIndex)
	{
		using Blam::Objects::ObjectData;

		if (p_EquipmentIndex == 0xFFFF)
			return 0;

		// Checks if dual-wielding and disables equipment use if so
		if (*(uint8_t *)(0x244D33D) == 0)
			return 0;

		auto *s_PlayerObjectData = (uint8_t *)(*ObjectData::GetDataArray())[p_PlayerObjectIndex].Data;
		
		return *(s_PlayerObjectData + 0x320 + p_EquipmentIndex);
	}

	__declspec(naked) void SprintInputHook()
	{
		__asm
		{
			mov		ecx, edi
			cmp		byte ptr ds : [0244D33Dh], 0	; zero if dual wielding
			jne		enable							; leave sprint enabled(for now) if not dual wielding
			and		ax, 0FEFFh						; disable by removing the 8th bit indicating no sprint input press
		enable:
			mov		dword ptr ds : [esi + 8], eax
			push	046DFC0h
			ret
		}
	}
	
	__declspec(naked) void ScopeLevelHook()
	{
		__asm
		{
			// scope level is an int16 with -1 indicating no scope, 0 indicating first level, 1 indicating second level etc.
			mov		word ptr ds:[edi + esi + 32Ah], 0FFFFh	; no scope by default
			cmp		byte ptr ds:[0244D33Dh], 0				; zero if dual wielding
			je		noscope									; prevent scoping when dual wielding
			mov		word ptr ds:[edi + esi + 32Ah], ax		; otherwise use intended scope level
		noscope:
			push	05D50D3h
			ret
		}
	}
	
	__declspec(naked) void HostObjectHealthHook()
	{
		__asm
		{
			pushad

			; get tls info
			mov		eax, dword ptr fs:[02Ch]	; tls array address
			mov		eax, dword ptr ds:[eax]		; slot 0 tls address

			; get local player object offset
			mov		ebx, dword ptr ds:[eax + 05Ch]	; local player mappings
			mov		ebx, dword ptr ds:[ebx + 014h]	; local player datum
			and		ebx, 0FFFFh						; local player index
			shl		ebx, 4							; multiply by object entry size of 16 bytes
			add		ebx, 0Ch						; add object header size

			; get local player object data address
			mov		ecx, dword ptr ds:[eax + 0448h]	; object header address
			mov		ecx, dword ptr ds:[ecx + 044h]	; first object address
			add		ecx, ebx						; local player object address
			mov		ecx, [ecx]						; local player object data address

			; check if damaging local player object and descope if so
			cmp		edi, ecx
			jne		orig

			; descope local player
			mov		edx, dword ptr ds:[eax + 0C4h]		; player control globals
			mov		word ptr ds:[edx + 032Ah], 0FFFFh	; descope

		orig:
			popad
			ucomiss	xmm1, xmm0
			movss   dword ptr ds:[edi + 100h], xmm1
			push	0B553ABh
			ret
		}
	}
	
	__declspec(naked) void HostObjectShieldHook()
	{
		__asm
		{
			pushad

			; get tls info
			mov		eax, dword ptr fs:[02Ch]	; tls array address
			mov		eax, dword ptr ds:[eax]		; slot 0 tls address

			; get local player object offset
			mov		ebx, dword ptr ds:[eax + 05Ch]	; local player mappings
			mov		ebx, dword ptr ds:[ebx + 014h]	; local player datum
			and		ebx, 0FFFFh						; local player index
			shl		ebx, 4							; multiply by object entry size of 16 bytes
			add		ebx, 0Ch						; add object header size

			; get local player object data address
			mov		ecx, dword ptr ds:[eax + 0448h]	; object header address
			mov		ecx, dword ptr ds:[ecx + 044h]	; first object address
			add		ecx, ebx						; local player object address
			mov		ecx, [ecx]						; local player object data address

			; check if damaging local player object and descope if so
			cmp		edi, ecx
			jne		orig

			; descope local player
			mov		edx, dword ptr ds:[eax + 0C4h]		; player control globals
			mov		word ptr ds:[edx + 032Ah], 0FFFFh	; descope

		orig:
			popad
			movss   dword ptr ds:[edi + 0FCh], xmm1
			push	0B54B56h
			ret
		}
	}
	
	__declspec(naked) void ClientObjectHealthHook()
	{
		__asm
		{
			pushad

			; get tls info
			mov		eax, dword ptr fs:[02Ch]	; tls array address
			mov		eax, dword ptr ds:[eax]		; slot 0 tls address

			; get local player object offset
			mov		ebx, dword ptr ds:[eax + 05Ch]	; local player mappings
			mov		ebx, dword ptr ds:[ebx + 014h]	; local player datum
			and		ebx, 0FFFFh						; local player index
			shl		ebx, 4							; multiply by object entry size of 16 bytes
			add		ebx, 0Ch						; add object header size

			; get local player object data address
			mov		ecx, dword ptr ds:[eax + 0448h]	; object header address
			mov		ecx, dword ptr ds:[ecx + 044h]	; first object address
			add		ecx, ebx						; local player object address
			mov		ecx, [ecx]						; local player object data address

			; check if damaging local player object
			cmp		edi, ecx
			jne		orig

			; only descope if shield is decreasing at a rate greater than epsilon
			mov		esi, 03C23D70Ah						; use an epsilon of 0.01f
			movd	xmm7, esi
			movss	xmm6, dword ptr ds:[edi + 100h]		; get original health
			subss	xmm6, xmm0							; get negative health delta (orig - new)
			comiss	xmm6, xmm7							; compare health delta with epsilon
			jb		orig								; skip descope if delta is less than epsilon

			; descope local player
			mov		edx, dword ptr ds:[eax + 0C4h]		; player control globals
			mov		word ptr ds:[edx + 032Ah], 0FFFFh	; descope

		orig:
			popad
			movss   dword ptr ds:[edi + 100h], xmm0
			push	0B33F1Bh
			ret
		}
	}
	
	__declspec(naked) void ClientObjectShieldHook()
	{
		__asm
		{
			pushad

			; get tls info
			mov		eax, dword ptr fs:[02Ch]	; tls array address
			mov		eax, dword ptr ds:[eax]		; slot 0 tls address

			; get local player object offset
			mov		ebx, dword ptr ds:[eax + 05Ch]	; local player mappings
			mov		ebx, dword ptr ds:[ebx + 014h]	; local player datum
			and		ebx, 0FFFFh						; local player index
			shl		ebx, 4							; multiply by object entry size of 16 bytes
			add		ebx, 0Ch						; add object header size

			; get local player object data address
			mov		edx, dword ptr ds:[eax + 0448h]	; object header address
			mov		edx, dword ptr ds:[edx + 044h]	; first object address
			add		edx, ebx						; local player object address
			mov		edx, [edx]						; local player object data address

			; check if damaging local player object
			cmp		ecx, edx
			jne		orig

			; only descope if shield is decreasing at a rate greater than epsilon
			mov		esi, 03C23D70Ah						; use an epsilon of 0.01f
			movd	xmm7, esi
			movss	xmm6, dword ptr ds:[ecx + 0FCh]		; get original shield
			subss	xmm6, xmm0							; get negative shield delta (orig - new)
			comiss	xmm6, xmm7							; compare shield delta with epsilon
			jb		orig								; skip descope if delta is less than epsilon

			; descope local player
			mov		edx, dword ptr ds:[eax + 0C4h]		; player control globals
			mov		word ptr ds:[edx + 032Ah], 0FFFFh	; descope

		orig:
			popad
			movss   dword ptr ds:[ecx + 0FCh], xmm0
			push	0B329D6h
			ret
		}
	}
	
	void GrenadeLoadoutHookImpl(uint8_t *p_UnitData)
	{
		// Based off of 0x8227B48C in H3 non-TU

		// TODO: Clean this up, hardcoded offsets are hacky
		const size_t s_GrenadeCountOffset = 0x320;
		const size_t s_ControllingPlayerOffset = 0x198;
		auto *s_GrenadeCounts = p_UnitData + s_GrenadeCountOffset; // 0 = frag, 1 = plasma, 2 = spike, 3 = firebomb
		auto s_PlayerIndex = *reinterpret_cast<int16_t *>(p_UnitData + s_ControllingPlayerOffset);
		if (s_PlayerIndex < 0)
		{
			memset(s_GrenadeCounts, 0, 4);
			return;
		}

		// Get the player's grenade setting
		auto &s_Players = Blam::Game::GetPlayers();
		auto s_GrenadeSetting = s_Players[s_PlayerIndex].SpawnGrenadeSetting;

		// Get the current scenario tag
		auto *s_Scenario = Blam::Tags::Scenario::GetCurrentScenario();

		// If the setting is none (2) or the scenario has invalid starting
		// profile data, set the grenade counts to 0 and return
		if (s_GrenadeSetting == 2 || !s_Scenario->StartingProfile)
		{
			memset(s_GrenadeCounts, 0, 4);
			return;
		}

		// Load the grenade counts from the scenario tag
		auto *s_Profile = &s_Scenario->StartingProfile[0];
		s_GrenadeCounts[0] = s_Profile->FragGrenades;
		s_GrenadeCounts[1] = s_Profile->PlasmaGrenades;
		s_GrenadeCounts[2] = s_Profile->SpikeGrenades;
		s_GrenadeCounts[3] = s_Profile->FirebombGrenades;
	}

	__declspec(naked) void GrenadeLoadoutHook()
	{
		__asm
		{
			push edi // Unit object data
			call GrenadeLoadoutHookImpl
			add esp, 4
			push 0x5A32C7
			ret
		}
	}
	
	__declspec(naked) void AudioMaxChannelsHook()
	{
		__asm
		{
			push 0
			push ebx
			push 0x400 // increase max channels from 0x40 to 0x400
			push eax
			call dword ptr [ecx] // inits fmod
			push 0x404EA4
			ret
		}
	}

	__declspec(naked) void DedicatedServerHook()
	{
		__asm
		{
			cmp edi, 0xffffffff
			jz end
			movzx ecx, di
			mov eax, [eax+0x40]
			imul ecx, 0x2f08
			mov eax, [eax+0x44]
			mov edi, [eax+ecx+0x30]
		end:
			push 0xA2E709
			ret
		}
	}

	bool Engine::ApplyCorePatches()
	{
		using AnvilCommon::Utils::Util;

			// Enable tag edits
		return Util::PatchAddress(0x101A5B, "\xEB", 1)
			&& Util::PatchAddress(0x102874, "\x90\x90", 2)
			&& Util::PatchAddress(0x1030AA, "\x90\x90", 2)
			// Set game locale to english
			&& Util::PatchAddress(0x2333FD, "\x00", 1)
			// No --account args
			&& Util::PatchAddress(0x43731A, "\xEB\x0E", 2)
			&& Util::PatchAddress(0x4373AD, "\xEB\x03", 2)
			// Prevent game variant weapons from being overridden
			&& Util::PatchAddress(0x1A315F, "\xEB", 1)
			&& Util::PatchAddress(0x1A31A4, "\xEB", 1)
			// Map loading?
			&& Util::PatchAddress(0x2D26DF, "\x90\x90\x90\x90\x90", 5)
			// Add the FMOD WASAPI output fix from FMODEx 4.44.56
			&& Util::PatchAddress(0x100DA75, "\x02", 1)
			// Fix random colored lighting
			&& Util::PatchAddress(0x14F2FFC, "\x00\x00\x00\x00", 4)
			// Enable spawning AI through effects
			&& Util::PatchAddress(0x1033321, "\x90\x90", 2)
			// Prevent FOV from being overridden when the game loads
			&& Util::PatchAddress(0x25FA79, "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 10)
			&& Util::PatchAddress(0x25FA86, "\x90\x90\x90\x90\x90", 5)
			&& Util::ApplyHook(0x10CA02, FovHook)
			// Enable dual-wielding
			&& Util::ApplyHook(0x761550, DualWieldHook)
			// Update equipment count for dual-wielding
			&& Util::ApplyHook(0x7440F0, GetEquipmentCountHook)
			// Update sprint input for dual-wielding
			&& Util::ApplyHook(0x6DFBB, SprintInputHook)
			// Update scope level for dual-wielding
			&& Util::ApplyHook(0x1D50CB, ScopeLevelHook)
			// Fix descoping
			&& Util::ApplyHook(0x7553A0, HostObjectHealthHook)
			&& Util::ApplyHook(0x754B4E, HostObjectShieldHook)
			&& Util::ApplyHook(0x733F13, ClientObjectHealthHook)
			&& Util::ApplyHook(0x7329CE, ClientObjectShieldHook)
			// Fix grenade loadouts
			&& Util::ApplyHook(0x1A3267, GrenadeLoadoutHook)
			// Fix audio maximum channels
			&& Util::ApplyHook(0x4E9C, AudioMaxChannelsHook)
			// Fix dedicated servers
			&& Util::ApplyHook(0x62E6F3, DedicatedServerHook);
	}
}