//#include "Utils\Hook.hpp"
//#include "Utils\Patch.hpp"
//
//#include "Blam\Objects\ObjectData.hpp"
//
//#include "Forge.hpp"
//
//namespace AnvilEldorado
//{
//	const auto CheckKillTriggers = reinterpret_cast<bool(*)(int32_t, void *)>(0x68C410);
//
//	bool CheckKillTriggersHook(int32_t a0, void *a1)
//	{
//		auto *s_Forge = Forge::Instance();
//
//		if (!s_Forge->KillBarriersEnabled())
//			return false;
//
//		return CheckKillTriggers(a0, a1);
//	}
//
//	const auto CheckSafeZones = reinterpret_cast<bool(*)(void *)>(0x4EB130);
//
//	bool ObjectSafeZoneHook(void *a0)
//	{
//		auto *s_Forge = Forge::Instance();
//
//		s_Forge->UpdateBarriersEnabled();
//
//		if (!s_Forge->KillBarriersEnabled())
//			return true;
//
//		return CheckSafeZones(a0);
//	}
//
//	const auto GetStructureDesign = reinterpret_cast<void*(*)(int32_t)>(0x4E97D0);
//
//	void *PushBarriersGetStructureDesignHook(int32_t index)
//	{
//		auto *s_Forge = Forge::Instance();
//
//		s_Forge->UpdateBarriersEnabled();
//
//		if (!s_Forge->PushBarriersEnabled())
//			return nullptr; // Return a null sddt if push barriers are disabled
//
//		return GetStructureDesign(index);
//	}
//	
//	bool g_ShouldDelete = false;
//
//	__declspec(naked) void Forge_UpdateInput_Hook()
//	{
//		__asm
//		{
//			mov al, g_ShouldDelete
//			test al, al
//			jnz del
//
//			// Not deleting - just call the original function
//			push esi
//			mov eax, 0x59F0E0
//			call eax
//			retn 4
//
//		del:
//			mov g_ShouldDelete, 0
//
//			// Simulate a Y button press
//			mov eax, 0x244D1F0              // Controller data
//			mov byte ptr [eax + 0x9E], 1    // Ticks = 1
//			and byte ptr [eax + 0x9F], 0xFE // Clear the "handled" flag
//
//			// Call the original function
//			push esi
//			mov eax, 0x59F0E0
//			call eax
//
//			// Make sure nothing else gets the fake press
//			mov eax, 0x244D1F0          // Controller data
//			or byte ptr [eax + 0x9F], 1 // Set the "handled" flag
//			retn 4
//		}
//	}
//
//	bool Forge::Init()
//	{
//		using AnvilCommon::Utils::HookFlags;
//		using AnvilCommon::Utils::Hook;
//		using AnvilCommon::Utils::Patch;
//		
//		return Patch(0x19F17F, 0x75).Apply()
//			&& Patch::NopFill(0x19F198, 4)
//			&& Hook(0x19D482, Forge_UpdateInput_Hook, HookFlags::IsCall).Apply()
//			&& Hook(0x771C7D, CheckKillTriggersHook, HookFlags::IsCall).Apply()
//			&& Hook(0x7B4C32, CheckKillTriggersHook, HookFlags::IsCall).Apply()
//			&& Hook(0x19EBA1, ObjectSafeZoneHook, HookFlags::IsCall).Apply()
//			&& Hook(0x19FDBE, ObjectSafeZoneHook, HookFlags::IsCall).Apply()
//			&& Hook(0x19FEEC, ObjectSafeZoneHook, HookFlags::IsCall).Apply()
//			&& Hook(0x32663D, ObjectSafeZoneHook, HookFlags::IsCall).Apply()
//			&& Hook(0x2749D1, PushBarriersGetStructureDesignHook, HookFlags::IsCall).Apply()
//			&& Hook(0x274DBA, PushBarriersGetStructureDesignHook, HookFlags::IsCall).Apply()
//			&& Hook(0x2750F8, PushBarriersGetStructureDesignHook, HookFlags::IsCall).Apply()
//			&& Hook(0x275655, PushBarriersGetStructureDesignHook, HookFlags::IsCall).Apply();
//	}
//
//	void Forge::UpdateBarriersEnabled()
//	{
//		// Don't scan multiple times per tick
//		if (m_BarriersEnabledValid)
//			return;
//
//		// Scan the object table to check if the barrier disablers are spawned
//		auto *s_ObjectArray = Blam::Objects::ObjectData::GetDataArray();
//
//		if (!s_ObjectArray)
//			return;
//
//		m_KillBarriersEnabled = true;
//		m_PushBarriersEnabled = true;
//
//		for (auto &s_Object : *s_ObjectArray)
//		{
//			// The objects are identified by tag index.
//			// scen 0x5728 disables kill barriers
//			// scen 0x5729 disables push barriers
//			if (s_Object.Type != Blam::Objects::ObjectType::Scenery)
//				continue;
//
//			auto s_TagIndex = s_Object.GetTagIndex().Index();
//
//			if (s_TagIndex == 0x5728)
//				m_KillBarriersEnabled = false;
//			else if (s_TagIndex == 0x5729)
//				m_PushBarriersEnabled = false;
//
//			if (!m_KillBarriersEnabled && !m_PushBarriersEnabled)
//				break;
//		}
//
//		m_BarriersEnabledValid = true;
//	}
//
//	bool Forge::ShouldDelete() const
//	{
//		return g_ShouldDelete;
//	}
//
//	void Forge::SetShouldDelete(const bool p_ShouldDelete)
//	{
//		g_ShouldDelete = p_ShouldDelete;
//	}
//
//	bool Forge::BarriersEnabledValid() const
//	{
//		return m_BarriersEnabledValid;
//	}
//
//	void Forge::SetBarriersEnabledValid(const bool p_Valid)
//	{
//		m_BarriersEnabledValid = p_Valid;
//	}
//
//	bool Forge::KillBarriersEnabled() const
//	{
//		return m_KillBarriersEnabled;
//	}
//
//	void Forge::SetKillBarriersEnabled(const bool p_Enabled)
//	{
//		m_KillBarriersEnabled = p_Enabled;
//	}
//
//	bool Forge::PushBarriersEnabled() const
//	{
//		return m_PushBarriersEnabled;
//	}
//
//	void Forge::SetPushBarriersEnabled(const bool p_Enabled)
//	{
//		m_PushBarriersEnabled = p_Enabled;
//	}
//}