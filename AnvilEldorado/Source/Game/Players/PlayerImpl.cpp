#include "PlayerImpl.hpp"
#include "Hooks.hpp"

#include <Game\Cache\StringIdCache.hpp>
#include <Game\Players\PlayerArmorExtension.hpp>

#include <Interfaces\Client.hpp>
#include <EngineImpl.hpp>

#include <Globals.hpp>
#include <codecvt>

using namespace AnvilEldorado::Game::Players;

const size_t AnvilEldorado::Game::Players::PlayerPropertiesPacketHeaderSize = 0x18;
const size_t AnvilEldorado::Game::Players::PlayerPropertiesSize = 0x30;
const size_t AnvilEldorado::Game::Players::PlayerPropertiesPacketFooterSize = 0x4;

bool PlayerImpl::Init()
{
	m_PlayerPropertiesExtender->Add(std::make_shared<PlayerArmorExtension>());

	m_Hooks = std::make_shared<Players::Hooks>();
	if (!m_Hooks->Init())
		return false;

	return true;
}

Blam::Data::DataArray<Blam::Game::Players::PlayerDatum> &PlayerImpl::GetPlayers()
{
	return *AnvilCommon::GetThreadStorage<Blam::Data::DataArray<Blam::Game::Players::PlayerDatum>>(0x40);
}

Blam::Data::DatumIndex PlayerImpl::GetLocalPlayer(const int32_t p_Index)
{
	typedef uint32_t(*GetLocalPlayerPtr)(int index);
	auto GetLocalPlayer = reinterpret_cast<GetLocalPlayerPtr>(0x589C30);
	return GetLocalPlayer(p_Index);
}

size_t PlayerImpl::GetPlayerPropertiesPacketSize()
{
	static size_t size;

	if (size == 0)
	{
		size_t extensionSize = m_PlayerPropertiesExtender->GetTotalSize();
		size = PlayerPropertiesPacketHeaderSize + PlayerPropertiesSize + extensionSize + PlayerPropertiesPacketFooterSize;
	}

	return size;
}

uint8_t GetArmorIndex(Blam::Text::StringID p_Name, const std::map<Blam::Text::StringID, uint8_t> &p_Indices)
{
	auto it = p_Indices.find(p_Name);

	return (it != p_Indices.end()) ? it->second : 0;
}


bool AddArmorPermutations(const Blam::Tags::Game::MultiplayerGlobals::Universal::ArmorCustomization &p_Element, std::map<Blam::Text::StringID, uint8_t> &p_Map)
{

	for (auto i = 0; i < p_Element.Permutations.Count; i++)
	{
		auto &s_Permutation = p_Element.Permutations[i];

		if (!s_Permutation.FirstPersonArmorModel && !s_Permutation.ThirdPersonArmorObject)
			continue;

		p_Map.emplace(s_Permutation.Name.Value, i);
	}

	return true;
}

bool PlayerImpl::LoadArmor(Blam::Tags::Game::MultiplayerGlobals *p_MultiplayerGlobals)
{
	for (auto &s_Customization : p_MultiplayerGlobals->Universal->SpartanArmorCustomization)
	{
		auto s_PieceRegion = s_Customization.PieceRegion.Value;

		if (s_PieceRegion == 0x1CE7) // helmet
			AddArmorPermutations(s_Customization, m_ArmorHelmetIndices);
		else if (s_PieceRegion == 0x9E) // chest
			AddArmorPermutations(s_Customization, m_ArmorChestIndices);
		else if (s_PieceRegion == 0x35D8) // shoulders
			AddArmorPermutations(s_Customization, m_ArmorShouldersIndices);
		else if (s_PieceRegion == 0x606) // arms
			AddArmorPermutations(s_Customization, m_ArmorArmsIndices);
		else if (s_PieceRegion == 0x605) // legs
			AddArmorPermutations(s_Customization, m_ArmorLegsIndices);
		else if (s_PieceRegion == 0x35D9) // acc
			AddArmorPermutations(s_Customization, m_ArmorAccessoryIndices);
		else if (s_PieceRegion == 0x268) // pelvis
			AddArmorPermutations(s_Customization, m_ArmorPelvisIndices);
		else
		{
			WriteLog("ERROR: Invalid armor piece region: 0x%X", s_PieceRegion);
			return false;
		}
	}

	for (auto &s_Variant : p_MultiplayerGlobals->Universal->GameVariantWeapons)
	{
		if (s_Variant.Weapon.TagIndex != 0xFFFF)
			m_PodiumWeaponIndices.emplace(s_Variant.Name.Value, s_Variant.Weapon.TagIndex);
	}

	m_ArmorPrimaryColor = 0xFFFFFF;
	m_ArmorSecondaryColor = 0xFFFFFF;
	m_ArmorVisorColor = 0xFFFFFF;
	m_ArmorLightsColor = 0xFFFFFF;
	m_ArmorHoloColor = 0xFFFFFF;

	m_ArmorHelmet = 0x358F; // air_assault
	m_ArmorChest = 0x358F; // air_assault
	m_ArmorShoulders = 0x358F; // air_assault
	m_ArmorArms = 0x358F; // air_assault
	m_ArmorLegs = 0x358F; // air_assault
	m_ArmorPelvis = 0x358F; // air_assault
	m_ArmorAccessory = 0x0;

	m_UpdateArmor = true;

	return true;
}

Blam::Data::DatumIndex PlayerImpl::GetPodiumBiped() const
{
	return Blam::Data::DatumIndex(*reinterpret_cast<uint32_t *>((uint8_t *)AnvilCommon::Internal_GetModuleStorage() + 0x4BE67A0));
}

void PlayerImpl::BuildCustomization(Blam::Game::Players::PlayerCustomization *p_Customization) const
{
	memset(p_Customization, 0, sizeof(Blam::Game::Players::PlayerCustomization));

	memset(p_Customization->Colors, 0, 5 * sizeof(uint32_t));

	p_Customization->Colors[(int)Blam::Game::Players::PlayerColor::Primary] = m_ArmorPrimaryColor;
	p_Customization->Colors[(int)Blam::Game::Players::PlayerColor::Secondary] = m_ArmorSecondaryColor;
	p_Customization->Colors[(int)Blam::Game::Players::PlayerColor::Visor] = m_ArmorVisorColor;
	p_Customization->Colors[(int)Blam::Game::Players::PlayerColor::Lights] = m_ArmorLightsColor;
	p_Customization->Colors[(int)Blam::Game::Players::PlayerColor::Holo] = m_ArmorHoloColor;

	p_Customization->Armor[(int)Blam::Game::Players::PlayerArmor::Helmet] = GetArmorIndex(m_ArmorHelmet, m_ArmorHelmetIndices);
	p_Customization->Armor[(int)Blam::Game::Players::PlayerArmor::Chest] = GetArmorIndex(m_ArmorChest, m_ArmorChestIndices);
	p_Customization->Armor[(int)Blam::Game::Players::PlayerArmor::Shoulders] = GetArmorIndex(m_ArmorShoulders, m_ArmorShouldersIndices);
	p_Customization->Armor[(int)Blam::Game::Players::PlayerArmor::Arms] = GetArmorIndex(m_ArmorArms, m_ArmorArmsIndices);
	p_Customization->Armor[(int)Blam::Game::Players::PlayerArmor::Legs] = GetArmorIndex(m_ArmorLegs, m_ArmorLegsIndices);
	p_Customization->Armor[(int)Blam::Game::Players::PlayerArmor::Acc] = GetArmorIndex(m_ArmorAccessory, m_ArmorAccessoryIndices);
	p_Customization->Armor[(int)Blam::Game::Players::PlayerArmor::Pelvis] = GetArmorIndex(m_ArmorPelvis, m_ArmorPelvisIndices);
}

const auto Biped_ApplyArmor = reinterpret_cast<void(*)(Blam::Game::Players::PlayerCustomization *, uint32_t)>(0x5A4430);
const auto Biped_ApplyArmorColor = reinterpret_cast<void(*)(uint32_t, int32_t, float *)>(0xB328F0);
const auto Biped_UpdateArmorColors = reinterpret_cast<void(*)(uint32_t)>(0x5A2FA0);

/*void Biped_PoseWithWeapon(uint32_t p_Unit, uint32_t p_Weapon)
{
	const auto sub_B450F0 = reinterpret_cast<char(*)(uint16_t, int)>(0xB450F0);
	const auto sub_B31590 = reinterpret_cast<DWORD*(*)(void*, int, int, int)>(0xB31590);
	const auto sub_B30440 = reinterpret_cast<signed int(*)(double, __m128, void*)>(0xB30440);
	const auto sub_B393D0 = reinterpret_cast<char(*)(double, __m128, int, int, int)>(0xB393D0);
	const auto sub_B2CD10 = reinterpret_cast<int(*)(double, __m128, int)>(0xB2CD10);

	if (!sub_B450F0(p_Unit, p_Weapon))
	{
		char v9;
		sub_B31590(&v9, p_Weapon, -1, 0);
		signed int v7 = sub_B30440(a1, a2, &v9);//crash
		if (v7 != -1 && !sub_B393D0(a1, a2, p_Unit, v7, 8))
			sub_B2CD10(a1, a2, v7);
	}
}*/

//TODO: Remove inlined assembly (WIP version above)
__declspec(naked) void Biped_PoseWithWeapon(uint32_t p_Unit, uint32_t p_Weapon)
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

void PlayerImpl::CustomizeBiped(const Blam::Data::DatumIndex &p_BipedIndex)
{
	// Generate customization data
	Blam::Game::Players::PlayerCustomization customization;
	BuildCustomization(&customization);

	// Apply armor to the biped
	Biped_ApplyArmor(&customization, p_BipedIndex.Value);

	// Apply each color
	for (int32_t i = 0; i < (int)Blam::Game::Players::PlayerColor::Count; i++)
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
		s_WeaponName = (m_PodiumWeapon = 0x41A); // assault_rifle

	Biped_PoseWithWeapon(p_BipedIndex.Value, m_PodiumWeaponIndices.find(s_WeaponName)->second);
}

std::wstring PlayerImpl::GetName() const
{
	return m_UserName;
}

void PlayerImpl::SetName(const std::string &p_UserName)
{
	m_UserName = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>().from_bytes(p_UserName.c_str());
	m_UserName.resize(17);
	m_UserName.back() = 0;
}

bool PlayerImpl::NeedsArmorUpdate() const
{
	return m_UpdateArmor;
}

void PlayerImpl::SetUpdateArmor(const bool p_UpdateArmor)
{
	m_UpdateArmor = p_UpdateArmor;
}

uint32_t PlayerImpl::GetArmorPrimaryColor() const
{
	return m_ArmorPrimaryColor;
}

void PlayerImpl::SetArmorPrimaryColor(const uint32_t p_Color)
{
	m_ArmorPrimaryColor = p_Color;
}

void PlayerImpl::SetArmorPrimaryColor(const uint8_t p_Red, const uint8_t p_Green, const uint8_t p_Blue)
{
	SetArmorPrimaryColor((p_Blue << 16) | (p_Green << 8) | (p_Red << 0));
}

uint32_t PlayerImpl::GetArmorSecondaryColor() const
{
	return m_ArmorSecondaryColor;
}

void PlayerImpl::SetArmorSecondaryColor(const uint32_t p_Color)
{
	m_ArmorSecondaryColor = p_Color;
}

void PlayerImpl::SetArmorSecondaryColor(const uint8_t p_Red, const uint8_t p_Green, const uint8_t p_Blue)
{
	SetArmorSecondaryColor((p_Blue << 16) | (p_Green << 8) | (p_Red << 0));
}

uint32_t PlayerImpl::GetArmorVisorColor() const
{
	return m_ArmorVisorColor;
}

void PlayerImpl::SetArmorVisorColor(const uint32_t p_Color)
{
	m_ArmorVisorColor = p_Color;
}

void PlayerImpl::SetArmorVisorColor(const uint8_t p_Red, const uint8_t p_Green, const uint8_t p_Blue)
{
	SetArmorVisorColor((p_Blue << 16) | (p_Green << 8) | (p_Red << 0));
}

uint32_t PlayerImpl::GetArmorLightsColor() const
{
	return m_ArmorLightsColor;
}

void PlayerImpl::SetArmorLightsColor(const uint32_t p_Color)
{
	m_ArmorLightsColor = p_Color;
}

void PlayerImpl::SetArmorLightsColor(const uint8_t p_Red, const uint8_t p_Green, const uint8_t p_Blue)
{
	SetArmorLightsColor((p_Blue << 16) | (p_Green << 8) | (p_Red << 0));
}

uint32_t PlayerImpl::GetArmorHoloColor() const
{
	return m_ArmorHoloColor;
}

void PlayerImpl::SetArmorHoloColor(const uint32_t p_Color)
{
	m_ArmorHoloColor = p_Color;
}

void PlayerImpl::SetArmorHoloColor(const uint8_t p_Red, const uint8_t p_Green, const uint8_t p_Blue)
{
	SetArmorHoloColor((p_Blue << 16) | (p_Green << 8) | (p_Red << 0));
}

Blam::Text::StringID PlayerImpl::GetArmorHelmet() const
{
	return m_ArmorHelmet;
}

void PlayerImpl::SetArmorHelmet(const Blam::Text::StringID &p_ArmorHelmet)
{
	m_ArmorHelmet = p_ArmorHelmet;
}

Blam::Text::StringID PlayerImpl::GetArmorChest() const
{
	return m_ArmorChest;
}

void PlayerImpl::SetArmorChest(const Blam::Text::StringID &p_ArmorChest)
{
	m_ArmorChest = p_ArmorChest;
}

Blam::Text::StringID PlayerImpl::GetArmorShoulders() const
{
	return m_ArmorShoulders;
}

void PlayerImpl::SetArmorShoulders(const Blam::Text::StringID &p_ArmorShoulders)
{
	m_ArmorShoulders = p_ArmorShoulders;
}

Blam::Text::StringID PlayerImpl::GetArmorArms() const
{
	return m_ArmorArms;
}

void PlayerImpl::SetArmorArms(const Blam::Text::StringID &p_ArmorArms)
{
	m_ArmorArms = p_ArmorArms;
}

Blam::Text::StringID PlayerImpl::GetArmorLegs() const
{
	return m_ArmorLegs;
}

void PlayerImpl::SetArmorLegs(const Blam::Text::StringID &p_ArmorLegs)
{
	m_ArmorLegs = p_ArmorLegs;
}

Blam::Text::StringID PlayerImpl::GetArmorPelvis() const
{
	return m_ArmorPelvis;
}

void PlayerImpl::SetArmorPelvis(const Blam::Text::StringID &p_ArmorPelvis)
{
	m_ArmorPelvis = p_ArmorPelvis;
}

Blam::Text::StringID PlayerImpl::GetArmorAccessory() const
{
	return m_ArmorAccessory;
}

void PlayerImpl::SetArmorAccessory(const Blam::Text::StringID &p_ArmorAccessory)
{
	m_ArmorAccessory = p_ArmorAccessory;
}

Blam::Text::StringID PlayerImpl::GetPodiumWeapon() const
{
	return m_PodiumWeapon;
}

void PlayerImpl::SetPodiumWeapon(const Blam::Text::StringID &p_PodiumWeapon)
{
	m_PodiumWeapon = p_PodiumWeapon;
}