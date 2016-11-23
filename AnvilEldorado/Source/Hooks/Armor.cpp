#include <map>
#include <Windows.h>
#include "Utils\Util.hpp"
#include "Blam\Cache\StringIDCache.hpp"
#include "Blam\Game\Players.hpp"
#include "Blam\Tags\TagInstance.hpp"
#include "Blam\Tags\Game\Globals.hpp"
#include "Blam\Tags\Game\MultiplayerGlobals.hpp"
#include "Hook.hpp"

using namespace AnvilCommon::Utils;
using namespace Blam::Game;

namespace AnvilEldorado::Hooks
{
	std::map<std::string, uint8_t> helmetIndices;
	std::map<std::string, uint8_t> chestIndices;
	std::map<std::string, uint8_t> shouldersIndices;
	std::map<std::string, uint8_t> armsIndices;
	std::map<std::string, uint8_t> legsIndices;
	std::map<std::string, uint8_t> accIndices;
	std::map<std::string, uint8_t> pelvisIndices;
	std::map<std::string, uint16_t> weaponIndices;

	uint8_t GetArmorIndex(const std::string &p_Name, const std::map<std::string, uint8_t> &p_Indices)
	{
		auto it = p_Indices.find(p_Name);
		return (it != p_Indices.end()) ? it->second : 0;
	}

	using Blam::Cache::StringIDCache;
	using Blam::Tags::Game::MultiplayerGlobals;

	void AddArmorPermutations(const MultiplayerGlobals::Universal::ArmorCustomization &element, std::map<std::string, uint8_t> &map)
	{
		for (auto i = 0; i < element.Permutations.Count; i++)
		{
			auto &perm = element.Permutations[i];

			if (!perm.FirstPersonArmorModel && !perm.ThirdPersonArmorObject)
				continue;

			auto permName = std::string(StringIDCache::Instance.GetString(perm.Name));

			map.emplace(permName, i);
		}
	}

	uint8_t ValidateArmorPiece(std::map<std::string, uint8_t> indices, uint8_t index)
	{
		for (auto pair : indices)
		{
			if (pair.second == index)
				return index;
		}

		return 0;
	}

	void Armor_AfterTagsLoaded()
	{
		using Blam::Tags::TagInstance;
		using Blam::Tags::Game::Globals;
		using Blam::Tags::Game::MultiplayerGlobals;

		auto *matg = TagInstance(0x0016).GetDefinition<Globals>();
		MessageBox(NULL, "Globals tag loaded!", "Tags", MB_OK);

		auto *mulg = TagInstance(matg->MultiplayerGlobals.TagIndex).GetDefinition<MultiplayerGlobals>();
		MessageBox(NULL, "MultiplayerGlobals tag loaded!", "Tags", MB_OK);

		for (auto &element : mulg->Universal->SpartanArmorCustomization)
		{
			auto string = std::string(StringIDCache::Instance.GetString(element.PieceRegion));

			if (string == "helmet")
				AddArmorPermutations(element, helmetIndices);
			else if (string == "chest")
				AddArmorPermutations(element, chestIndices);
			else if (string == "shoulders")
				AddArmorPermutations(element, shouldersIndices);
			else if (string == "arms")
				AddArmorPermutations(element, armsIndices);
			else if (string == "legs")
				AddArmorPermutations(element, legsIndices);
			else if (string == "acc")
				AddArmorPermutations(element, accIndices);
			else if (string == "pelvis")
				AddArmorPermutations(element, pelvisIndices);
			else
				throw std::exception("Invalid armor section");
		}

		for (auto &element : mulg->Universal->GameVariantWeapons)
		{
			auto index = (uint16_t)element.Weapon.TagIndex;

			if (index == 0xFFFF)
				continue;
			
			weaponIndices.emplace(std::string(StringIDCache::Instance.GetString(element.Name)), index);
		}
	}

	void BuildPlayerCustomization(PlayerCustomization *out)
	{
		memset(out, 0, sizeof(PlayerCustomization));

		memset(out->Colors, 0, 5 * sizeof(uint32_t));

		out->Colors[(int)PlayerColor::Primary] = 0xFFFFFFFF;
		out->Colors[(int)PlayerColor::Secondary] = 0xFFFFFFFF;
		out->Colors[(int)PlayerColor::Lights] = 0xFFFFFFFF;
		out->Colors[(int)PlayerColor::Visor] = 0xFFFFFFFF;
		out->Colors[(int)PlayerColor::Holo] = 0xFFFFFFFF;

		out->Armor[(int)PlayerArmor::Helmet] = GetArmorIndex("stealth", helmetIndices);
		out->Armor[(int)PlayerArmor::Chest] = GetArmorIndex("stealth", chestIndices);
		out->Armor[(int)PlayerArmor::Shoulders] = GetArmorIndex("stealth", shouldersIndices);
		out->Armor[(int)PlayerArmor::Arms] = GetArmorIndex("stealth", armsIndices);
		out->Armor[(int)PlayerArmor::Legs] = GetArmorIndex("stealth", legsIndices);
		out->Armor[(int)PlayerArmor::Acc] = GetArmorIndex("stealth", accIndices);
		out->Armor[(int)PlayerArmor::Pelvis] = GetArmorIndex("stealth", pelvisIndices);
	}

	__declspec(naked) void PoseWithWeapon(uint32_t unit, uint32_t weaponTag)
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
			mov esi, unit
			mov edi, weaponTag
			push 0x7B77DA
			ret
		}
	}

	void CustomizeBiped(uint32_t bipedObject)
	{
		// Generate customization data
		PlayerCustomization customization;
		BuildPlayerCustomization(&customization);

		// Apply armor to the biped
		reinterpret_cast<void(*)(PlayerCustomization *, uint32_t)>(0x5A4430)(&customization, bipedObject);

		// Apply each color
		for (int i = 0; i < (int)PlayerColor::Count; i++)
		{
			// Convert the color data from RGB to float3
			float colorData[3];
			reinterpret_cast<void(*)(uint32_t, float *)>(0x521300)(customization.Colors[i], colorData);

			// Apply the color
			reinterpret_cast<void(*)(uint32_t, int, float *)>(0xB328F0)(bipedObject, i, colorData);
		}

		// Need to call this or else colors don't actually show up
		reinterpret_cast<void(*)(uint32_t)>(0x5A2FA0)(bipedObject);

		auto weaponName = "assault_rifle";

		if (weaponIndices.find(weaponName) == weaponIndices.end())
			weaponName = "assault_rifle";

		PoseWithWeapon(bipedObject, weaponIndices.find(weaponName)->second);
	}

	bool updateUiPlayerArmor = false;
	
	void Armor_RefreshUiPlayer()
	{
		updateUiPlayerArmor = true;
	}

	void UiPlayerModelArmorHook()
	{
		// This function runs every tick, so only update if necessary
		if (!updateUiPlayerArmor)
			return;

		// Try to get the UI player biped
		auto s_BaseAddress = reinterpret_cast<size_t>(GetModuleHandle(nullptr));
		auto uiPlayerBiped = *((uint32_t *)(s_BaseAddress + 0x4BE67A0));
		if (uiPlayerBiped == 0xFFFFFFFF)
			return;

		CustomizeBiped(uiPlayerBiped);
		updateUiPlayerArmor = false;

		// Note: the call that this hook replaces is for setting up armor based on the server data,
		// so it's not necessary to call here
	}

	void Hook::Hook_Armor()
	{
		// Fix the player model on the main menu
		Util::ApplyHook(0x20086D, UiPlayerModelArmorHook, HookFlags::IsCall);
	}
}