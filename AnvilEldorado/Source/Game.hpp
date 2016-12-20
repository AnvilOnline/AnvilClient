#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "Interfaces\IInitializable.hpp"
#include "Utils\Singleton.hpp"
#include "Blam\Game\Variants.hpp"

namespace AnvilEldorado
{
	enum class GameLocale : int8_t
	{
		English,            // en-US
		Japanese,           // jp-JP
		German,             // de-DE
		French,             // fr-FR
		Spanish,            // es-ES
		Mexican,            // es-MX
		Italian,            // it-IT
		Korean,             // ko-KR
		ChineseTraditional, // zh-TW
		ChineseSimplified,  // zh-CN
		Portuguese,         // pt-PT
		Russian             // ru-RU
	};

	enum class GameLobbyType : int32_t
	{
		None,
		Campaign,
		CustomGame,
		Forge
	};

	class Game final : public AnvilCommon::IInitializable, public AnvilCommon::Singleton<Game>
	{
	public:
		bool Init() override;

		GameLocale GetGameLocale() const;
		bool SetGameLocale(const GameLocale &p_Locale);

		void Shutdown();

		bool StartGame();
		bool EndGame();
		
		int GetMapID(const std::string &p_MapName);

		bool LoadMap(const std::string &p_MapName);
		bool LoadMapVariant(std::ifstream &p_File, uint8_t *out);
		bool LoadDefaultMapVariant(const std::string &p_MapPath, uint8_t *out);
		void SaveMapVariantToPreferences(const uint8_t *p_Data);

		GameLobbyType GetLobbyType() const;

		const std::vector<std::string> &GetMaps() const;
		const std::vector<std::string> &GetMapVariants() const;

	private:
		GameLocale m_Locale;
		std::vector<std::string> m_Maps;
		std::vector<std::string> m_MapVariants;
	};
}