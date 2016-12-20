#include <codecvt>
#include <map>
#include "Blam\Cache\StringIDCache.hpp"
#include "Blam\Tags\TagInstance.hpp"
#include "Blam\Tags\Game\CacheFileGlobalTags.hpp"
#include "Blam\Tags\Game\Globals.hpp"
#include "Blam\Tags\Scenario\Scenario.hpp"
#include "Utils\Logger.hpp"
#include "Utils\Hook.hpp"
#include "Utils\Patch.hpp"
#include "Player.hpp"

namespace AnvilEldorado
{
	void PlayerArmorExtension::BuildData(int32_t playerIndex, Blam::Game::PlayerCustomization *out)
	{
		Player::Instance()->BuildCustomization(out);
	}

	void PlayerArmorExtension::ApplyData(int32_t playerIndex, Blam::Game::PlayerProperties *properties, const Blam::Game::PlayerCustomization &data)
	{
		auto armorSessionData = &properties->Customization;

		armorSessionData->Armor[(int)Blam::Game::PlayerArmor::Helmet] = data.Armor[(int)Blam::Game::PlayerArmor::Helmet];
		armorSessionData->Armor[(int)Blam::Game::PlayerArmor::Chest] = data.Armor[(int)Blam::Game::PlayerArmor::Chest];
		armorSessionData->Armor[(int)Blam::Game::PlayerArmor::Shoulders] = data.Armor[(int)Blam::Game::PlayerArmor::Shoulders];
		armorSessionData->Armor[(int)Blam::Game::PlayerArmor::Arms] = data.Armor[(int)Blam::Game::PlayerArmor::Arms];
		armorSessionData->Armor[(int)Blam::Game::PlayerArmor::Legs] = data.Armor[(int)Blam::Game::PlayerArmor::Legs];
		armorSessionData->Armor[(int)Blam::Game::PlayerArmor::Acc] = data.Armor[(int)Blam::Game::PlayerArmor::Acc];
		armorSessionData->Armor[(int)Blam::Game::PlayerArmor::Pelvis] = data.Armor[(int)Blam::Game::PlayerArmor::Pelvis];

		memcpy(armorSessionData->Colors, data.Colors, sizeof(data.Colors));
	}

	void PlayerArmorExtension::Serialize(Blam::Data::BitStream *stream, const Blam::Game::PlayerCustomization &data)
	{
		// Colors
		for (int32_t i = 0; i < (int)Blam::Game::PlayerColor::Count; i++)
			stream->WriteUnsigned<uint32_t>(data.Colors[i], 24);

		// Armor
		for (int32_t i = 0; i < (int)Blam::Game::PlayerArmor::Count; i++)
			stream->WriteUnsigned<uint8_t>(data.Armor[i], 0, UINT8_MAX);
	}

	void PlayerArmorExtension::Deserialize(Blam::Data::BitStream *stream, Blam::Game::PlayerCustomization *out)
	{
		memset(out, 0, sizeof(Blam::Game::PlayerCustomization));

		// Colors
		for (int32_t i = 0; i < (int)Blam::Game::PlayerColor::Count; i++)
			out->Colors[i] = stream->ReadUnsigned<uint32_t>(24);

		// Armor
		for (int32_t i = 0; i < (int)Blam::Game::PlayerArmor::Count; i++)
			out->Armor[i] = stream->ReadUnsigned<uint8_t>(0, UINT8_MAX);
	}

	__declspec(naked) void Biped_PoseWithWeapon_Hook(uint32_t p_Unit, uint32_t p_Weapon)
	{
		// This is a pretty big hack, basically I don't know where the function pulls the weapon index from
		// so this lets us skip over the beginning of the function and set the weapon tag to whatever we want
		__asm
		{
			push ebp
			mov ebp, esp
			sub esp, 0x18C
			push esi
			push edi
			sub esp, 0x8
			mov esi, p_Unit
			mov edi, p_Weapon
			push 0x7B77DA
			ret
		}
	}

	void UiPlayerModelArmorHook()
	{
		auto *s_Player = Player::Instance();

		// This function runs every tick, so only update if necessary
		if (!s_Player->NeedsArmorUpdate())
			return;

		// Try to get the UI player biped
		auto s_UiPlayerBiped = s_Player->GetPodiumBiped();
		if (s_UiPlayerBiped == 0xFFFFFFFF)
			return;

		s_Player->CustomizeBiped(s_UiPlayerBiped);
		s_Player->SetUpdateArmor(false);

		// Note: the call that this hook replaces is for setting up armor based on the server data,
		// so it's not necessary to call here
	}

	uint32_t GetScoreboardPlayerBiped()
	{
		return *reinterpret_cast<uint32_t *>((uint8_t *)AnvilCommon::Internal_GetModuleStorage() + 0x4C4C698);
	}

	void ScoreboardPlayerModelArmorHook()
	{
		auto *s_Player = Player::Instance();

		auto s_ScoreboardBiped = GetScoreboardPlayerBiped();

		if (s_ScoreboardBiped != 0xFFFFFFFF)
			s_Player->CustomizeBiped(s_ScoreboardBiped);
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

	bool Player::Init()
	{
		using AnvilCommon::Utils::HookFlags;
		using AnvilCommon::Utils::Hook;
		using AnvilCommon::Utils::Patch;

		Blam::Game::PlayerPropertiesExtender::Instance()->Add(std::make_shared<PlayerArmorExtension>());

		return Hook(0x20086D, UiPlayerModelArmorHook, HookFlags::IsCall).Apply()
			&& Patch::NopFill(0x435DAB, 0x50)
			&& Hook(0x4360D9, ScoreboardPlayerModelArmorHook, HookFlags::IsCall).Apply()
			&& Patch::NopFill(0x4360DE, 0x1A9)
			&& Patch(0x43628A, 0x1C).Apply()
			&& Patch::NopFill(0x43628B, 0x3)
			// Fix grenade loadouts
			&& Hook(0x1A3267, GrenadeLoadoutHook).Apply();
	}

	bool Player::OnTagsLoaded()
	{
		using Blam::Tags::TagInstance;
		using Blam::Tags::Game::CacheFileGlobalTags;
		using Blam::Tags::Game::Globals;
		using Blam::Tags::Game::MultiplayerGlobals;

		auto *s_CacheFileGlobalTags = TagInstance(0).GetDefinition<CacheFileGlobalTags>();
		Globals *s_Globals = nullptr;

		for (auto &s_GlobalTag : s_CacheFileGlobalTags->GlobalTags)
		{
			if (s_GlobalTag.GroupTag == 'matg')
			{
				s_Globals = TagInstance(s_GlobalTag.TagIndex).GetDefinition<Globals>();
				break;
			}
		}

		if (s_Globals == nullptr)
		{
			WriteLog("ERROR: Failed to locate globals tag (matg) in cache file global tags (cfgt)!");
			return false;
		}

		auto *s_MultiplayerGlobals = TagInstance(s_Globals->MultiplayerGlobals.TagIndex).GetDefinition<MultiplayerGlobals>();

		if (s_MultiplayerGlobals == nullptr)
		{
			WriteLog("ERROR: Failed to locate multiplayer globals tag (mulg) in globals tag (matg)!");
			return false;
		}

		return LoadArmor(s_MultiplayerGlobals)
			&& LoadPodiumWeapons(s_MultiplayerGlobals);
	}

	bool AddArmorPermutations(const Blam::Tags::Game::MultiplayerGlobals::Universal::ArmorCustomization &p_Element, std::map<std::string, uint8_t> &p_Map)
	{
		auto *s_StringIDs = Blam::Cache::StringIDCache::Instance();

		for (auto i = 0; i < p_Element.Permutations.Count; i++)
		{
			auto &s_Permutation = p_Element.Permutations[i];

			if (!s_Permutation.FirstPersonArmorModel && !s_Permutation.ThirdPersonArmorObject)
				continue;

			p_Map.emplace(std::string(s_StringIDs->GetString(s_Permutation.Name)), i);
		}

		return true;
	}

	bool Player::LoadArmor(Blam::Tags::Game::MultiplayerGlobals *p_MultiplayerGlobals)
	{
		auto *s_StringIDs = Blam::Cache::StringIDCache::Instance();

		for (auto &s_Customization : p_MultiplayerGlobals->Universal->SpartanArmorCustomization)
		{
			auto s_PieceRegion = std::string(s_StringIDs->GetString(s_Customization.PieceRegion));

			if (s_PieceRegion == "helmet")
				AddArmorPermutations(s_Customization, m_ArmorHelmetIndices);
			else if (s_PieceRegion == "chest")
				AddArmorPermutations(s_Customization, m_ArmorChestIndices);
			else if (s_PieceRegion == "shoulders")
				AddArmorPermutations(s_Customization, m_ArmorShouldersIndices);
			else if (s_PieceRegion == "arms")
				AddArmorPermutations(s_Customization, m_ArmorArmsIndices);
			else if (s_PieceRegion == "legs")
				AddArmorPermutations(s_Customization, m_ArmorLegsIndices);
			else if (s_PieceRegion == "acc")
				AddArmorPermutations(s_Customization, m_ArmorAccessoryIndices);
			else if (s_PieceRegion == "pelvis")
				AddArmorPermutations(s_Customization, m_ArmorPelvisIndices);
			else
			{
				WriteLog("ERROR: Invalid armor piece region: %s", s_PieceRegion.c_str());
				return false;
			}
		}

		m_ArmorPrimaryColor = 0xFFFFFF;
		m_ArmorSecondaryColor = 0xFFFFFF;
		m_ArmorVisorColor = 0xFFFFFF;
		m_ArmorLightsColor = 0xFFFFFF;
		m_ArmorHoloColor = 0xFFFFFF;

		m_ArmorHelmet = "air_assault";
		m_ArmorChest = "air_assault";
		m_ArmorShoulders = "air_assault";
		m_ArmorArms = "air_assault";
		m_ArmorLegs = "air_assault";
		m_ArmorPelvis = "air_assault";
		m_ArmorAccessory = "";

		m_UpdateArmor = true;

		return true;
	}

	bool Player::LoadPodiumWeapons(Blam::Tags::Game::MultiplayerGlobals *p_MultiplayerGlobals)
	{
		auto *s_StringIDs = Blam::Cache::StringIDCache::Instance();

		for (auto &s_Variant : p_MultiplayerGlobals->Universal->GameVariantWeapons)
		{
			auto s_Name = std::string(s_StringIDs->GetString(s_Variant.Name));
			auto s_Index = (uint16_t)s_Variant.Weapon.TagIndex;

			if (s_Index != 0xFFFF)
				m_PodiumWeaponIndices.emplace(s_Name, s_Index);
		}

		return true;
	}

	uint8_t GetArmorIndex(const std::string &p_Name, const std::map<std::string, uint8_t> &p_Indices)
	{
		auto it = p_Indices.find(p_Name);

		return (it != p_Indices.end()) ? it->second : 0;
	}

	Blam::Data::DatumIndex Player::GetPodiumBiped() const
	{
		return Blam::Data::DatumIndex(*reinterpret_cast<uint32_t *>((uint8_t *)AnvilCommon::Internal_GetModuleStorage() + 0x4BE67A0));
	}

	void Player::BuildCustomization(Blam::Game::PlayerCustomization *p_Customization) const
	{
		memset(p_Customization, 0, sizeof(Blam::Game::PlayerCustomization));

		memset(p_Customization->Colors, 0, 5 * sizeof(uint32_t));

		p_Customization->Colors[(int)Blam::Game::PlayerColor::Primary] = m_ArmorPrimaryColor;
		p_Customization->Colors[(int)Blam::Game::PlayerColor::Secondary] = m_ArmorSecondaryColor;
		p_Customization->Colors[(int)Blam::Game::PlayerColor::Visor] = m_ArmorVisorColor;
		p_Customization->Colors[(int)Blam::Game::PlayerColor::Lights] = m_ArmorLightsColor;
		p_Customization->Colors[(int)Blam::Game::PlayerColor::Holo] = m_ArmorHoloColor;

		p_Customization->Armor[(int)Blam::Game::PlayerArmor::Helmet] = GetArmorIndex(m_ArmorHelmet, m_ArmorHelmetIndices);
		p_Customization->Armor[(int)Blam::Game::PlayerArmor::Chest] = GetArmorIndex(m_ArmorChest, m_ArmorChestIndices);
		p_Customization->Armor[(int)Blam::Game::PlayerArmor::Shoulders] = GetArmorIndex(m_ArmorShoulders, m_ArmorShouldersIndices);
		p_Customization->Armor[(int)Blam::Game::PlayerArmor::Arms] = GetArmorIndex(m_ArmorArms, m_ArmorArmsIndices);
		p_Customization->Armor[(int)Blam::Game::PlayerArmor::Legs] = GetArmorIndex(m_ArmorLegs, m_ArmorLegsIndices);
		p_Customization->Armor[(int)Blam::Game::PlayerArmor::Acc] = GetArmorIndex(m_ArmorAccessory, m_ArmorAccessoryIndices);
		p_Customization->Armor[(int)Blam::Game::PlayerArmor::Pelvis] = GetArmorIndex(m_ArmorPelvis, m_ArmorPelvisIndices);
	}

	const auto Biped_ApplyArmor = reinterpret_cast<void(*)(Blam::Game::PlayerCustomization *, uint32_t)>(0x5A4430);
	const auto Biped_ApplyArmorColor = reinterpret_cast<void(*)(uint32_t, int32_t, float *)>(0xB328F0);
	const auto Biped_UpdateArmorColors = reinterpret_cast<void(*)(uint32_t)>(0x5A2FA0);

	void Player::CustomizeBiped(const Blam::Data::DatumIndex &p_BipedIndex)
	{
		// Generate customization data
		Blam::Game::PlayerCustomization customization;
		BuildCustomization(&customization);

		// Apply armor to the biped
		Biped_ApplyArmor(&customization, p_BipedIndex.Value);

		// Apply each color
		for (int32_t i = 0; i < (int)Blam::Game::PlayerColor::Count; i++)
		{
			// Convert the color data from RGB to float3
			float colorData[3];
			typedef void(*RgbToFloatColorPtr)(uint32_t rgbColor, float *result);
			RgbToFloatColorPtr RgbToFloatColor = reinterpret_cast<RgbToFloatColorPtr>(0x521300);
			RgbToFloatColor(customization.Colors[i], colorData);

			// Apply the color
			Biped_ApplyArmorColor(p_BipedIndex.Value, i, colorData);
		}

		// Need to call this or else colors don't actually show up
		Biped_UpdateArmorColors(p_BipedIndex.Value);

		// Give the biped a weapon (0x151E = tag index for Assault Rifle)
		auto s_WeaponName = m_PodiumWeapon;

		if (m_PodiumWeaponIndices.find(s_WeaponName) == m_PodiumWeaponIndices.end())
			s_WeaponName = (m_PodiumWeapon = "assault_rifle");

		Biped_PoseWithWeapon_Hook(p_BipedIndex.Value, m_PodiumWeaponIndices.find(s_WeaponName)->second);
	}

	std::wstring Player::GetUserName() const
	{
		return m_UserName;
	}

	void Player::SetUserName(const std::string &p_UserName)
	{
		m_UserName = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>().from_bytes(p_UserName.c_str());
		m_UserName.resize(17);
		m_UserName.back() = 0;
	}

	bool Player::NeedsArmorUpdate() const
	{
		return m_UpdateArmor;
	}

	void Player::SetUpdateArmor(const bool p_UpdateArmor)
	{
		m_UpdateArmor = p_UpdateArmor;
	}

	uint32_t Player::GetArmorPrimaryColor() const
	{
		return m_ArmorPrimaryColor;
	}

	void Player::SetArmorPrimaryColor(const uint32_t p_Color)
	{
		m_ArmorPrimaryColor = p_Color;
	}

	void Player::SetArmorPrimaryColor(const uint8_t p_Red, const uint8_t p_Green, const uint8_t p_Blue)
	{
		SetArmorPrimaryColor((p_Blue << 16) | (p_Green << 8) | (p_Red << 0));
	}

	uint32_t Player::GetArmorSecondaryColor() const
	{
		return m_ArmorSecondaryColor;
	}

	void Player::SetArmorSecondaryColor(const uint32_t p_Color)
	{
		m_ArmorSecondaryColor = p_Color;
	}

	void Player::SetArmorSecondaryColor(const uint8_t p_Red, const uint8_t p_Green, const uint8_t p_Blue)
	{
		SetArmorSecondaryColor((p_Blue << 16) | (p_Green << 8) | (p_Red << 0));
	}

	uint32_t Player::GetArmorVisorColor() const
	{
		return m_ArmorVisorColor;
	}

	void Player::SetArmorVisorColor(const uint32_t p_Color)
	{
		m_ArmorVisorColor = p_Color;
	}

	void Player::SetArmorVisorColor(const uint8_t p_Red, const uint8_t p_Green, const uint8_t p_Blue)
	{
		SetArmorVisorColor((p_Blue << 16) | (p_Green << 8) | (p_Red << 0));
	}

	uint32_t Player::GetArmorLightsColor() const
	{
		return m_ArmorLightsColor;
	}

	void Player::SetArmorLightsColor(const uint32_t p_Color)
	{
		m_ArmorLightsColor = p_Color;
	}

	void Player::SetArmorLightsColor(const uint8_t p_Red, const uint8_t p_Green, const uint8_t p_Blue)
	{
		SetArmorLightsColor((p_Blue << 16) | (p_Green << 8) | (p_Red << 0));
	}

	uint32_t Player::GetArmorHoloColor() const
	{
		return m_ArmorHoloColor;
	}

	void Player::SetArmorHoloColor(const uint32_t p_Color)
	{
		m_ArmorHoloColor = p_Color;
	}

	void Player::SetArmorHoloColor(const uint8_t p_Red, const uint8_t p_Green, const uint8_t p_Blue)
	{
		SetArmorHoloColor((p_Blue << 16) | (p_Green << 8) | (p_Red << 0));
	}

	std::string Player::GetArmorHelmet() const
	{
		return m_ArmorHelmet;
	}

	void Player::SetArmorHelmet(const std::string &p_ArmorHelmet)
	{
		m_ArmorHelmet = p_ArmorHelmet;
	}

	std::string Player::GetArmorChest() const
	{
		return m_ArmorChest;
	}

	void Player::SetArmorChest(const std::string &p_ArmorChest)
	{
		m_ArmorChest = p_ArmorChest;
	}

	std::string Player::GetArmorShoulders() const
	{
		return m_ArmorShoulders;
	}

	void Player::SetArmorShoulders(const std::string &p_ArmorShoulders)
	{
		m_ArmorShoulders = p_ArmorShoulders;
	}

	std::string Player::GetArmorArms() const
	{
		return m_ArmorArms;
	}

	void Player::SetArmorArms(const std::string &p_ArmorArms)
	{
		m_ArmorArms = p_ArmorArms;
	}

	std::string Player::GetArmorLegs() const
	{
		return m_ArmorLegs;
	}

	void Player::SetArmorLegs(const std::string &p_ArmorLegs)
	{
		m_ArmorLegs = p_ArmorLegs;
	}

	std::string Player::GetArmorPelvis() const
	{
		return m_ArmorPelvis;
	}

	void Player::SetArmorPelvis(const std::string &p_ArmorPelvis)
	{
		m_ArmorPelvis = p_ArmorPelvis;
	}

	std::string Player::GetArmorAccessory() const
	{
		return m_ArmorAccessory;
	}

	void Player::SetArmorAccessory(const std::string &p_ArmorAccessory)
	{
		m_ArmorAccessory = p_ArmorAccessory;
	}

	std::string Player::GetPodiumWeapon() const
	{
		return m_PodiumWeapon;
	}

	void Player::SetPodiumWeapon(const std::string &p_PodiumWeapon)
	{
		m_PodiumWeapon = p_PodiumWeapon;
	}
}