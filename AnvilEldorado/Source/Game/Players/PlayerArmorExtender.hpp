#include <Blam/Game/Players/PlayerCustomization.hpp>
#include <Game/Players/PlayerPropertiesExtension.hpp>

namespace AnvilEldorado::Game::Players
{
	class PlayerArmorExtension :
		public PlayerPropertiesExtension<Blam::Game::Players::PlayerCustomization>
	{
	protected:
		void BuildData(int32_t playerIndex, Blam::Game::Players::PlayerCustomization *out) override;
		void ApplyData(int32_t playerIndex, Blam::Game::Players::PlayerProperties *properties, const Blam::Game::Players::PlayerCustomization &data) override;
		void Serialize(Blam::Data::BitStream *stream, const Blam::Game::Players::PlayerCustomization &data) override;
		void Deserialize(Blam::Data::BitStream *stream, Blam::Game::Players::PlayerCustomization *out) override;
	};
}
