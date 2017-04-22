#pragma once
#include <Interfaces\Initializable.hpp>

#include <Blam\Data\DataArray.hpp>
#include <Blam\Data\DatumIndex.hpp>
#include <Blam\Game\Players\PlayerDatum.hpp>
#include <Blam\Tags\Game\MultiplayerGlobals.hpp>

#include <Game\Players\PlayerPropertiesExtender.hpp>

#include <map>

namespace AnvilEldorado::Game::Players
{
	class PlayerImpl :
		public AnvilCommon::Initializable
	{
	protected:
		std::shared_ptr<AnvilCommon::Initializable> m_Hooks;

	private:
		PlayerPropertiesExtender* m_PlayerPropertiesExtender = new PlayerPropertiesExtender();;

		std::wstring m_UserName;

		bool m_UpdateArmor = false;

		uint32_t m_ArmorPrimaryColor;
		uint32_t m_ArmorSecondaryColor;
		uint32_t m_ArmorVisorColor;
		uint32_t m_ArmorLightsColor;
		uint32_t m_ArmorHoloColor;

		std::map<Blam::Text::StringID, uint8_t> m_ArmorHelmetIndices;
		Blam::Text::StringID m_ArmorHelmet;

		std::map<Blam::Text::StringID, uint8_t> m_ArmorChestIndices;
		Blam::Text::StringID m_ArmorChest;

		std::map<Blam::Text::StringID, uint8_t> m_ArmorShouldersIndices;
		Blam::Text::StringID m_ArmorShoulders;

		std::map<Blam::Text::StringID, uint8_t> m_ArmorArmsIndices;
		Blam::Text::StringID m_ArmorArms;

		std::map<Blam::Text::StringID, uint8_t> m_ArmorLegsIndices;
		Blam::Text::StringID m_ArmorLegs;

		std::map<Blam::Text::StringID, uint8_t> m_ArmorPelvisIndices;
		Blam::Text::StringID m_ArmorPelvis;

		std::map<Blam::Text::StringID, uint8_t> m_ArmorAccessoryIndices;
		Blam::Text::StringID m_ArmorAccessory;

		std::map<Blam::Text::StringID, uint16_t> m_PodiumWeaponIndices;
		Blam::Text::StringID m_PodiumWeapon;

	public:
		virtual bool Init();

		Blam::Data::DataArray<Blam::Game::Players::PlayerDatum> &GetPlayers();
		Blam::Data::DatumIndex GetLocalPlayer(const int32_t p_Index);
		size_t GetPlayerPropertiesPacketSize();

		bool LoadArmor(Blam::Tags::Game::MultiplayerGlobals *p_MultiplayerGlobals);

		Blam::Data::DatumIndex GetPodiumBiped() const;

		void BuildCustomization(Blam::Game::Players::PlayerCustomization * p_Customization) const;
		void CustomizeBiped(const Blam::Data::DatumIndex &p_BipedIndex);

		std::wstring GetName() const;
		void SetName(const std::string &p_UserName);

		bool NeedsArmorUpdate() const;
		void SetUpdateArmor(const bool p_UpdateArmor = true);

		uint32_t GetArmorPrimaryColor() const;
		void SetArmorPrimaryColor(const uint32_t p_Color);
		void SetArmorPrimaryColor(const uint8_t p_Red, const uint8_t p_Green, const uint8_t p_Blue);

		uint32_t GetArmorSecondaryColor() const;
		void SetArmorSecondaryColor(const uint32_t p_Color);
		void SetArmorSecondaryColor(const uint8_t p_Red, const uint8_t p_Green, const uint8_t p_Blue);

		uint32_t GetArmorVisorColor() const;
		void SetArmorVisorColor(const uint32_t p_Color);
		void SetArmorVisorColor(const uint8_t p_Red, const uint8_t p_Green, const uint8_t p_Blue);

		uint32_t GetArmorLightsColor() const;
		void SetArmorLightsColor(const uint32_t p_Color);
		void SetArmorLightsColor(const uint8_t p_Red, const uint8_t p_Green, const uint8_t p_Blue);

		uint32_t GetArmorHoloColor() const;
		void SetArmorHoloColor(const uint32_t p_Color);
		void SetArmorHoloColor(const uint8_t p_Red, const uint8_t p_Green, const uint8_t p_Blue);

		Blam::Text::StringID GetArmorHelmet() const;
		void SetArmorHelmet(const Blam::Text::StringID &p_ArmorHelmet);

		Blam::Text::StringID GetArmorChest() const;
		void SetArmorChest(const Blam::Text::StringID &p_ArmorChest);

		Blam::Text::StringID GetArmorShoulders() const;
		void SetArmorShoulders(const Blam::Text::StringID &p_ArmorShoulders);

		Blam::Text::StringID GetArmorArms() const;
		void SetArmorArms(const Blam::Text::StringID &p_ArmorArms);

		Blam::Text::StringID GetArmorLegs() const;
		void SetArmorLegs(const Blam::Text::StringID &p_ArmorLegs);

		Blam::Text::StringID GetArmorPelvis() const;
		void SetArmorPelvis(const Blam::Text::StringID &p_ArmorPelvis);

		Blam::Text::StringID GetArmorAccessory() const;
		void SetArmorAccessory(const Blam::Text::StringID &p_ArmorAccessory);

		Blam::Text::StringID GetPodiumWeapon() const;
		void SetPodiumWeapon(const Blam::Text::StringID &p_PodiumWeapon);
	};

	// Packet size constants
	extern const size_t PlayerPropertiesPacketHeaderSize;
	extern const size_t PlayerPropertiesSize;
	extern const size_t PlayerPropertiesPacketFooterSize;
}
