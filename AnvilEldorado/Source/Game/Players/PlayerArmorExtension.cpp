#include "PlayerArmorExtender.hpp"
#include "PlayerImpl.hpp";

#include <EngineImpl.hpp>
#include <Interfaces/Client.hpp>

using namespace AnvilEldorado::Game::Players;

void PlayerArmorExtension::BuildData(int32_t p_PlayerIndex, Blam::Game::Players::PlayerCustomization *p_Out)
{
	GetClientInterface()->GetEngine()->GetSubsystem<PlayerImpl>()->BuildCustomization(p_Out);
}

void PlayerArmorExtension::ApplyData(int32_t p_PlayerIndex, Blam::Game::Players::PlayerProperties *p_Properties, const Blam::Game::Players::PlayerCustomization &p_Data)
{
	auto armorSessionData = &p_Properties->Customization;

	armorSessionData->Armor[(int)Blam::Game::Players::PlayerArmor::Helmet] = p_Data.Armor[(int)Blam::Game::Players::PlayerArmor::Helmet];
	armorSessionData->Armor[(int)Blam::Game::Players::PlayerArmor::Chest] = p_Data.Armor[(int)Blam::Game::Players::PlayerArmor::Chest];
	armorSessionData->Armor[(int)Blam::Game::Players::PlayerArmor::Shoulders] = p_Data.Armor[(int)Blam::Game::Players::PlayerArmor::Shoulders];
	armorSessionData->Armor[(int)Blam::Game::Players::PlayerArmor::Arms] = p_Data.Armor[(int)Blam::Game::Players::PlayerArmor::Arms];
	armorSessionData->Armor[(int)Blam::Game::Players::PlayerArmor::Legs] = p_Data.Armor[(int)Blam::Game::Players::PlayerArmor::Legs];
	armorSessionData->Armor[(int)Blam::Game::Players::PlayerArmor::Acc] = p_Data.Armor[(int)Blam::Game::Players::PlayerArmor::Acc];
	armorSessionData->Armor[(int)Blam::Game::Players::PlayerArmor::Pelvis] = p_Data.Armor[(int)Blam::Game::Players::PlayerArmor::Pelvis];

	memcpy(armorSessionData->Colors, p_Data.Colors, sizeof(p_Data.Colors));
}

void PlayerArmorExtension::Serialize(Blam::Data::BitStream *p_Stream, const Blam::Game::Players::PlayerCustomization &p_Data)
{
	// Colors
	for (int32_t i = 0; i < (int)Blam::Game::Players::PlayerColor::Count; i++)
		p_Stream->WriteUnsigned<uint32_t>(p_Data.Colors[i], 24);

	// Armor
	for (int32_t i = 0; i < (int)Blam::Game::Players::PlayerArmor::Count; i++)
		p_Stream->WriteUnsigned<uint8_t>(p_Data.Armor[i], 0, UINT8_MAX);
}

void PlayerArmorExtension::Deserialize(Blam::Data::BitStream *p_Stream, Blam::Game::Players::PlayerCustomization *p_Out)
{
	memset(p_Out, 0, sizeof(Blam::Game::Players::PlayerCustomization));

	// Colors
	for (int32_t i = 0; i < (int)Blam::Game::Players::PlayerColor::Count; i++)
		p_Out->Colors[i] = p_Stream->ReadUnsigned<uint32_t>(24);

	// Armor
	for (int32_t i = 0; i < (int)Blam::Game::Players::PlayerArmor::Count; i++)
		p_Out->Armor[i] = p_Stream->ReadUnsigned<uint8_t>(0, UINT8_MAX);
}
