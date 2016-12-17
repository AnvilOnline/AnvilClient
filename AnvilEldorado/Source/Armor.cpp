#include <cstdint>
#include <map>
#include <string>
#include <sstream>
#include "Globals.hpp"
#include "Utils\Logger.hpp"
#include "Blam\Cache\StringIDCache.hpp"
#include "Blam\Game\Players.hpp"
#include "Blam\Tags\TagInstance.hpp"
#include "Blam\Tags\Game\CacheFileGlobalTags.hpp"
#include "Blam\Tags\Game\Globals.hpp"
#include "Blam\Tags\Game\MultiplayerGlobals.hpp"
#include "Engine.hpp"

namespace AnvilEldorado
{
	bool g_PlayerArmor_Update = false;

	void RefreshUiPlayer()
	{
		g_PlayerArmor_Update = true;
	}

	std::map<std::string, uint8_t> g_PlayerArmor_HelmetIndices;
	std::map<std::string, uint8_t> g_PlayerArmor_ChestIndices;
	std::map<std::string, uint8_t> g_PlayerArmor_ShouldersIndices;
	std::map<std::string, uint8_t> g_PlayerArmor_ArmsIndices;
	std::map<std::string, uint8_t> g_PlayerArmor_LegsIndices;
	std::map<std::string, uint8_t> g_PlayerArmor_AccessoryIndices;
	std::map<std::string, uint8_t> g_PlayerArmor_PelvisIndices;
	std::map<std::string, uint16_t> g_PlayerArmor_WeaponIndices;

	std::string g_Player_Helmet = "air_assault";
	std::string g_Player_Chest = "dutch";
	std::string g_Player_Shoulders = "dutch";
	std::string g_Player_Arms = "stealth";
	std::string g_Player_Legs = "stealth";
	std::string g_Player_Accessory = "";
	std::string g_Player_Pelvis = "";

	std::string g_Player_MannequinWeapon = "assault_rifle";

	uint32_t g_Player_PrimaryColor = 0x3D3D3D;
	uint32_t g_Player_SecondaryColor = 0x3D7777;
	uint32_t g_Player_VisorColor = 0x77663D;
	uint32_t g_Player_LightsColor = 0x000000;
	uint32_t g_Player_HoloColor = 0x000000;

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

	bool LoadPlayerArmor(Blam::Tags::Game::MultiplayerGlobals *p_MultiplayerGlobals)
	{
		auto *s_StringIDs = Blam::Cache::StringIDCache::Instance();

		for (auto &s_Customization : p_MultiplayerGlobals->Universal->SpartanArmorCustomization)
		{
			auto s_PieceRegion = std::string(s_StringIDs->GetString(s_Customization.PieceRegion));

			if (s_PieceRegion == "helmet")
				AddArmorPermutations(s_Customization, g_PlayerArmor_HelmetIndices);
			else if (s_PieceRegion == "chest")
				AddArmorPermutations(s_Customization, g_PlayerArmor_ChestIndices);
			else if (s_PieceRegion == "shoulders")
				AddArmorPermutations(s_Customization, g_PlayerArmor_ShouldersIndices);
			else if (s_PieceRegion == "arms")
				AddArmorPermutations(s_Customization, g_PlayerArmor_ArmsIndices);
			else if (s_PieceRegion == "legs")
				AddArmorPermutations(s_Customization, g_PlayerArmor_LegsIndices);
			else if (s_PieceRegion == "acc")
				AddArmorPermutations(s_Customization, g_PlayerArmor_AccessoryIndices);
			else if (s_PieceRegion == "pelvis")
				AddArmorPermutations(s_Customization, g_PlayerArmor_PelvisIndices);
			else
			{
				WriteLog("ERROR: Invalid armor piece region: %s", s_PieceRegion.c_str());
				return false;
			}
		}

		g_PlayerArmor_Update = true;

		return true;
	}

	bool LoadPlayerWeapons(Blam::Tags::Game::MultiplayerGlobals *p_MultiplayerGlobals)
	{
		auto *s_StringIDs = Blam::Cache::StringIDCache::Instance();

		for (auto &s_Variant : p_MultiplayerGlobals->Universal->GameVariantWeapons)
		{
			auto s_Name = std::string(s_StringIDs->GetString(s_Variant.Name));
			auto s_Index = (uint16_t)s_Variant.Weapon.TagIndex;

			if (s_Index != 0xFFFF)
				g_PlayerArmor_WeaponIndices.emplace(s_Name, s_Index);
		}

		return true;
	}

	uint8_t GetArmorIndex(const std::string &name, const std::map<std::string, uint8_t> &Indices)
	{
		auto it = Indices.find(name);
		return (it != Indices.end()) ? it->second : 0;
	}

	uint8_t ValidateArmorIndex(std::map<std::string, uint8_t> indices, uint8_t index)
	{
		// Just do a quick check to see if the index has a key associated with it,
		// and force it to 0 if not
		for (auto pair : indices)
		{
			if (pair.second == index)
				return index;
		}

		return 0;
	}

	void BuildPlayerCustomization(Blam::Game::PlayerCustomization *p_Customization)
	{
		memset(p_Customization, 0, sizeof(Blam::Game::PlayerCustomization));

		memset(p_Customization->Colors, 0, 5 * sizeof(uint32_t));

		p_Customization->Colors[(int)Blam::Game::PlayerColor::Primary] = 0x3D3D3D;
		p_Customization->Colors[(int)Blam::Game::PlayerColor::Secondary] = 0x3D7777;
		p_Customization->Colors[(int)Blam::Game::PlayerColor::Visor] = 0x77663D;
		p_Customization->Colors[(int)Blam::Game::PlayerColor::Lights] = 0x000000;
		p_Customization->Colors[(int)Blam::Game::PlayerColor::Holo] = 0x000000;

		p_Customization->Armor[(int)Blam::Game::PlayerArmor::Helmet] = GetArmorIndex("air_assault", g_PlayerArmor_HelmetIndices);
		p_Customization->Armor[(int)Blam::Game::PlayerArmor::Chest] = GetArmorIndex("dutch", g_PlayerArmor_ChestIndices);
		p_Customization->Armor[(int)Blam::Game::PlayerArmor::Shoulders] = GetArmorIndex("dutch", g_PlayerArmor_ShouldersIndices);
		p_Customization->Armor[(int)Blam::Game::PlayerArmor::Arms] = GetArmorIndex("stealth", g_PlayerArmor_ArmsIndices);
		p_Customization->Armor[(int)Blam::Game::PlayerArmor::Legs] = GetArmorIndex("stealth", g_PlayerArmor_LegsIndices);
		p_Customization->Armor[(int)Blam::Game::PlayerArmor::Acc] = GetArmorIndex("", g_PlayerArmor_AccessoryIndices);
		p_Customization->Armor[(int)Blam::Game::PlayerArmor::Pelvis] = GetArmorIndex("", g_PlayerArmor_PelvisIndices);
	}

	class ArmorExtension : public Blam::Game::PlayerPropertiesExtension<Blam::Game::PlayerCustomization>
	{
	protected:
		void BuildData(int32_t playerIndex, Blam::Game::PlayerCustomization *out) override
		{
			BuildPlayerCustomization(out);
		}

		void ApplyData(int32_t playerIndex, Blam::Game::PlayerProperties *properties, const Blam::Game::PlayerCustomization &data) override
		{
			auto armorSessionData = &properties->Customization;
			armorSessionData->Armor[(int)Blam::Game::PlayerArmor::Helmet] = data.Armor[(int)Blam::Game::PlayerArmor::Helmet];
			armorSessionData->Armor[(int)Blam::Game::PlayerArmor::Chest] = data.Armor[(int)Blam::Game::PlayerArmor::Chest];
			armorSessionData->Armor[(int)Blam::Game::PlayerArmor::Shoulders] = data.Armor[(int)Blam::Game::PlayerArmor::Shoulders];
			armorSessionData->Armor[(int)Blam::Game::PlayerArmor::Arms] = ValidateArmorIndex(g_PlayerArmor_ArmsIndices, data.Armor[(int)Blam::Game::PlayerArmor::Arms]);
			armorSessionData->Armor[(int)Blam::Game::PlayerArmor::Legs] = ValidateArmorIndex(g_PlayerArmor_LegsIndices, data.Armor[(int)Blam::Game::PlayerArmor::Legs]);
			armorSessionData->Armor[(int)Blam::Game::PlayerArmor::Acc] = ValidateArmorIndex(g_PlayerArmor_AccessoryIndices, data.Armor[(int)Blam::Game::PlayerArmor::Acc]);
			armorSessionData->Armor[(int)Blam::Game::PlayerArmor::Pelvis] = ValidateArmorIndex(g_PlayerArmor_PelvisIndices, data.Armor[(int)Blam::Game::PlayerArmor::Pelvis]);
			memcpy(armorSessionData->Colors, data.Colors, sizeof(data.Colors));
		}

		void Serialize(Blam::Data::BitStream *stream, const Blam::Game::PlayerCustomization &data) override
		{
			// Colors
			for (int32_t i = 0; i < (int)Blam::Game::PlayerColor::Count; i++)
				stream->WriteUnsigned<uint32_t>(data.Colors[i], 24);

			// Armor
			for (int32_t i = 0; i < (int)Blam::Game::PlayerArmor::Count; i++)
				stream->WriteUnsigned<uint8_t>(data.Armor[i], 0, UINT8_MAX);
		}

		void Deserialize(Blam::Data::BitStream *stream, Blam::Game::PlayerCustomization *out) override
		{
			memset(out, 0, sizeof(Blam::Game::PlayerCustomization));

			// Colors
			for (int32_t i = 0; i < (int)Blam::Game::PlayerColor::Count; i++)
				out->Colors[i] = stream->ReadUnsigned<uint32_t>(24);

			// Armor
			for (int32_t i = 0; i < (int)Blam::Game::PlayerArmor::Count; i++)
				out->Armor[i] = stream->ReadUnsigned<uint8_t>(0, UINT8_MAX);
		}
	};

	__declspec(naked) void PoseWithWeapon(uint32_t p_Unit, uint32_t p_Weapon)
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

	const auto Biped_ApplyArmor = reinterpret_cast<void(*)(Blam::Game::PlayerCustomization *, uint32_t)>(0x5A4430);
	const auto Biped_ApplyArmorColor = reinterpret_cast<void(*)(uint32_t, int32_t, float *)>(0xB328F0);
	const auto Biped_UpdateArmorColors = reinterpret_cast<void(*)(uint32_t)>(0x5A2FA0);

	void CustomizeBiped(uint32_t bipedObject)
	{
		// Generate customization data
		Blam::Game::PlayerCustomization customization;
		BuildPlayerCustomization(&customization);

		// Apply armor to the biped
		Biped_ApplyArmor(&customization, bipedObject);

		// Apply each color
		for (int32_t i = 0; i < (int)Blam::Game::PlayerColor::Count; i++)
		{
			// Convert the color data from RGB to float3
			float colorData[3];
			typedef void(*RgbToFloatColorPtr)(uint32_t rgbColor, float *result);
			RgbToFloatColorPtr RgbToFloatColor = reinterpret_cast<RgbToFloatColorPtr>(0x521300);
			RgbToFloatColor(customization.Colors[i], colorData);

			// Apply the color
			Biped_ApplyArmorColor(bipedObject, i, colorData);
		}

		// Need to call this or else colors don't actually show up
		Biped_UpdateArmorColors(bipedObject);

		// Give the biped a weapon (0x151E = tag index for Assault Rifle)
		auto s_WeaponName = g_Player_MannequinWeapon;

		if (g_PlayerArmor_WeaponIndices.find(s_WeaponName) == g_PlayerArmor_WeaponIndices.end())
			s_WeaponName = (g_Player_MannequinWeapon = "assault_rifle");

		PoseWithWeapon(bipedObject, g_PlayerArmor_WeaponIndices.find(s_WeaponName)->second);
	}

	uint32_t GetUiPlayerBiped()
	{
		return *reinterpret_cast<uint32_t *>((uint8_t *)AnvilCommon::Internal_GetModuleStorage() + 0x4BE67A0);
	}

	void UiPlayerModelArmorHook()
	{
		// This function runs every tick, so only update if necessary
		if (!g_PlayerArmor_Update)
			return;

		// Try to get the UI player biped
		auto s_UiPlayerBiped = GetUiPlayerBiped();
		if (s_UiPlayerBiped == 0xFFFFFFFF)
			return;

		CustomizeBiped(s_UiPlayerBiped);
		g_PlayerArmor_Update = false;

		// Note: the call that this hook replaces is for setting up armor based on the server data,
		// so it's not necessary to call here
	}

	uint32_t GetScoreboardPlayerBiped()
	{
		return *reinterpret_cast<uint32_t *>((uint8_t *)AnvilCommon::Internal_GetModuleStorage() + 0x4C4C698);
	}

	void ScoreboardPlayerModelArmorHook()
	{
		auto s_ScoreboardBiped = GetScoreboardPlayerBiped();

		if (s_ScoreboardBiped != 0xFFFFFFFF)
			CustomizeBiped(s_ScoreboardBiped);
	}

	bool Engine::ApplyPatches_Armor()
	{
		using AnvilCommon::Utils::HookFlags;
		using AnvilCommon::Utils::Util;

		auto *s_ModuleBase = AnvilCommon::Internal_GetModuleStorage();

		Blam::Game::PlayerPropertiesExtender::Instance()->Add(std::make_shared<ArmorExtension>());

		return Util::ApplyHook(0x20086D, UiPlayerModelArmorHook, HookFlags::IsCall)
			&& Util::NopAddress(0x435DAB, 0x50)
			&& Util::ApplyHook(0x4360D9, ScoreboardPlayerModelArmorHook, HookFlags::IsCall)
			&& Util::NopAddress(0x4360DE, 0x1A9)
			&& ((*reinterpret_cast<uint8_t *>((uint8_t *)s_ModuleBase + 0x43628A) = 0x1C) != 0)
			&& Util::NopAddress(0x43628B, 0x3);

		return true;
	}

	bool Engine::OnTagsLoaded_Armor()
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

		return LoadPlayerArmor(s_MultiplayerGlobals)
			&& LoadPlayerWeapons(s_MultiplayerGlobals);
	}
}