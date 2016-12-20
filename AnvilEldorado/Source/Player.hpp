#pragma once
#include <cstdint>
#include <map>
#include "Interfaces\IInitializable.hpp"
#include "Utils\Singleton.hpp"
#include "Blam\Game\Players.hpp"
#include "Blam\Tags\Game\MultiplayerGlobals.hpp"

namespace AnvilEldorado
{
	class PlayerArmorExtension : public Blam::Game::PlayerPropertiesExtension<Blam::Game::PlayerCustomization>
	{
	protected:
		void BuildData(int32_t playerIndex, Blam::Game::PlayerCustomization *out) override;
		void ApplyData(int32_t playerIndex, Blam::Game::PlayerProperties *properties, const Blam::Game::PlayerCustomization &data) override;
		void Serialize(Blam::Data::BitStream *stream, const Blam::Game::PlayerCustomization &data) override;
		void Deserialize(Blam::Data::BitStream *stream, Blam::Game::PlayerCustomization *out) override;
	};

	class Player final : public AnvilCommon::IInitializable, public AnvilCommon::Singleton<Player>
	{
	public:
		bool Init() override;

		bool OnTagsLoaded();

		bool LoadArmor(Blam::Tags::Game::MultiplayerGlobals *p_MultiplayerGlobals);
		bool LoadWeapons(Blam::Tags::Game::MultiplayerGlobals *p_MultiplayerGlobals);

		void BuildCustomization(Blam::Game::PlayerCustomization *p_Customization) const;
		void CustomizeBiped(const uint32_t p_BipedObject);

		inline bool NeedsArmorUpdate() const
		{
			return m_UpdateArmor;
		}

		void SetUpdateArmor(const bool p_UpdateArmor = true)
		{
			m_UpdateArmor = p_UpdateArmor;
		}

	protected:
		bool m_UpdateArmor = false;

		wchar_t m_UserName[17];

		uint32_t m_ArmorPrimaryColor;
		uint32_t m_ArmorSecondaryColor;
		uint32_t m_ArmorVisorColor;
		uint32_t m_ArmorLightsColor;
		uint32_t m_ArmorHoloColor;

		std::map<std::string, uint8_t> m_ArmorHelmetIndices;
		std::string m_ArmorHelmet;

		std::map<std::string, uint8_t> m_ArmorChestIndices;
		std::string m_ArmorChest;

		std::map<std::string, uint8_t> m_ArmorShouldersIndices;
		std::string m_ArmorShoulders;

		std::map<std::string, uint8_t> m_ArmorArmsIndices;
		std::string m_ArmorArms;

		std::map<std::string, uint8_t> m_ArmorLegsIndices;
		std::string m_ArmorLegs;

		std::map<std::string, uint8_t> m_ArmorPelvisIndices;
		std::string m_ArmorPelvis;

		std::map<std::string, uint8_t> m_ArmorAccessoryIndices;
		std::string m_ArmorAccessory;

		std::map<std::string, uint16_t> m_WeaponIndices;
		std::string m_Weapon;
	};
}