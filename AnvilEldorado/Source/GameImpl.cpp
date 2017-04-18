#include "GameImpl.hpp"
#include <Globals.hpp>

#include <Blam\Network\Session.hpp>
#include <Blam\Tags\Tag.hpp>

#include <Utils\Hook.hpp>
#include <Utils\Patch.hpp>
#include <Utils\Logger.hpp>

namespace AnvilEldorado
{
	void ApplyMapNameFixes()
	{
		auto s_MapNamesAddress = *reinterpret_cast<uint32_t *>((uint8_t *)AnvilCommon::Internal_GetModuleStorage() + 0x149E2E0);
		if (!s_MapNamesAddress)
			return;

		// TODO: map out these global arrays, content items seems to use same format

		uint32_t s_MapNameCount = *reinterpret_cast<uint32_t *>(s_MapNamesAddress + 0x34);

		const wchar_t* s_Search[12] = { L"guardian", L"riverworld", L"s3d_avalanche", L"s3d_edge", L"s3d_reactor", L"s3d_turf", L"cyberdyne", L"chill", L"deadlock", L"bunkerworld", L"shrine", L"zanzibar" };
		const wchar_t* s_Names[12] = { L"Guardian", L"Valhalla", L"Diamondback", L"Edge", L"Reactor", L"Icebox", L"The Pit", L"Narrows", L"High Ground", L"Standoff", L"Sandtrap", L"Last Resort" };
		// TODO: Get names/descs using string ids? Seems the unic tags have descs for most of the maps
		const wchar_t* s_Descriptions[12] = {
			L"Millennia of tending has produced trees as ancient as the Forerunner structures they have grown around. 2-6 players.",
			L"The crew of V-398 barely survived their unplanned landing in this gorge...this curious gorge. 6-16 players.",
			L"Hot winds blow over what should be a dead moon. A reminder of the power Forerunners once wielded. 6-16 players.",
			L"The remote frontier world of Partition has provided this ancient databank with the safety of seclusion. 6-16 players.",
			L"Being constructed just prior to the Invasion, its builders had to evacuate before it was completed. 6-16 players.",
			L"Downtown Tyumen's Precinct 13 offers an ideal context for urban combat training. 4-10 players.",
			L"Software simulations are held in contempt by the veteran instructors who run these training facilities. 4-10 players.",
			L"Without cooling systems such as these, excess heat from the Ark's forges would render the construct uninhabitable. 2-8 players.",
			L"A relic of older conflicts, this base was reactivated after the New Mombasa Slipspace Event. 4-12 players.",
			L"Once, nearby telescopes listened for a message from the stars. Now, these silos contain our prepared response. 4-12 players.",
			L"Although the Brute occupiers have been driven from this ancient structure, they left plenty to remember them by. 6-16 players",
			L"Remote industrial sites like this one are routinely requisitioned and used as part of Spartan training exercises. 4-12 players."

		};

		for (uint32_t i = 0; i < s_MapNameCount; i++)
		{
			auto *s_MapNameAddress = reinterpret_cast<wchar_t *>(s_MapNamesAddress + 0x54 + (0x360 * i) + 0x8);
			auto *s_MapDescriptionAddress = reinterpret_cast<wchar_t *>(s_MapNamesAddress + 0x54 + (0x360 * i) + 0x8 + 0x40);

			wchar_t s_MapName[0x21] = { 0 };
			memcpy(s_MapName, s_MapNameAddress, 0x20);

			for (uint32_t y = 0; y < sizeof(s_Search) / sizeof(*s_Search); y++)
			{
				if (wcscmp(s_Search[y], s_MapName) == 0)
				{
					memset(s_MapNameAddress, 0, sizeof(wchar_t) * 0x20);
					wcscpy_s(s_MapNameAddress, 0x20, s_Names[y]);

					memset(s_MapDescriptionAddress, 0, sizeof(wchar_t) * 0x80);
					wcscpy_s(s_MapDescriptionAddress, 0x80, s_Descriptions[y]);

					break;
				}
			}
		}
	}

	void GameTickHook()
	{
		static bool s_AppliedFirstTickPatches = false;

		if (!s_AppliedFirstTickPatches)
		{
			ApplyMapNameFixes();
			s_AppliedFirstTickPatches = true;
		}

		// Call replaced function
		reinterpret_cast<void(*)()>(0x5547F0)();
	}

	void ShutdownHook()
	{
		//
		// TODO: Call any shutdown functions...
		//

		// Call the replaced function
		reinterpret_cast<void(*)()>(0x42E410)();
	}

	const auto LoadMap = reinterpret_cast<bool(*)(void *p_Data)>(0x566EF0);

	bool LoadMapHook(void *p_Data)
	{
		if (!LoadMap(p_Data))
			return false;

		// TODO: Fix
		return false;// Game::Instance()->OnMapLoaded(p_Data);
	}

	void EndGameHook()
	{
		auto *s_Session = Blam::Network::Session::Current();

		if (!s_Session || !s_Session->IsEstablished())
			return;

		if (s_Session->IsHost())
			Blam::Network::EndGame();
		else
			Blam::Network::LeaveGame();
	}

	bool GameImpl::Init()
	{
		using AnvilCommon::Utils::HookFlags;
		using AnvilCommon::Utils::Hook;
		using AnvilCommon::Utils::Patch;

			// Set game locale
		return SetGameLocale(GameLocale::English)
			// Hook game ticks
			&& Hook(0x105ABA, GameTickHook, HookFlags::IsCall).Apply()
			&& Hook(0x1063E6, GameTickHook, HookFlags::IsCall).Apply()
			// Run callbacks on engine shutdown
			&& Hook(0x2EBD7, ShutdownHook, HookFlags::IsCall).Apply()
			// Map loading
			&& Hook(0x10FC2C, LoadMapHook, HookFlags::IsCall).Apply()
			&& Hook(0x1671BE, LoadMapHook, HookFlags::IsCall).Apply()
			&& Hook(0x167B4F, LoadMapHook, HookFlags::IsCall).Apply()
			// Rewire $hq.MatchmakingLeaveQueue() to end the game
			&& Hook(0x3B6826, EndGameHook, HookFlags::IsCall).Apply()
			&& Patch::NopFill(0x3B682B, 1)
			// Prevent game variant weapons from being overridden
			&& Patch(0x1A315F, 0xEB).Apply()
			&& Patch(0x1A31A4, 0xEB).Apply()
			// Update countdown timer
			&& Patch(0x153708, 5).Apply() // player control
			&& Patch(0x153738, 9).Apply() // camera position
			&& Patch(0x1521D1, 9).Apply() // ui timer
			&& Patch(0x1536F0, 3).Apply(); // team notification
	}

	GameLocale GameImpl::GetGameLocale() const
	{
		return *(GameLocale *)((uint8_t *)AnvilCommon::Internal_GetModuleStorage() + 0x2333FD);
	}

	bool GameImpl::SetGameLocale(const GameLocale &p_Language)
	{
		using AnvilCommon::Utils::Patch;

		return Patch(0x2333FD, (uint8_t)p_Language).Apply();
	}
	
	void GameImpl::Shutdown()
	{
		// TODO: Call pre-shutdown callbacks...

		std::exit(0);
	}

	bool GameImpl::StartGame()
	{
		auto *s_Session = Blam::Network::Session::Current();

		if (!s_Session || !s_Session->Parameters.SetSessionMode(2))
		{
			WriteLog("ERROR: Unable to start the game!");
			return false;
		}

		WriteLog("Starting game...");
		return true;
	}

	bool GameImpl::EndGame()
	{
		auto *s_Session = Blam::Network::Session::Current();

		if (!s_Session || !s_Session->Parameters.SetSessionMode(2))
		{
			WriteLog("ERROR: Unable to end the game!");
			return false;
		}

		WriteLog("Ending game...");
		return true;
	}

	int32_t GameImpl::GetMapID(const std::string &p_MapName)
	{
		//
		// Open the .map file
		//

		auto s_MapPath = "maps\\" + p_MapName + ".map";

		std::ifstream s_MapFile(s_MapPath, std::ios::binary | std::ios::end);
		if (!s_MapFile.is_open())
			return -1;

		//
		// Verify map file's size
		//

		auto s_MapFileSize = s_MapFile.tellg();
		s_MapFile.seekg(0, std::ios::beg);

		if (s_MapFileSize < 0x3390)
			return -1;

		//
		// Verify the map file's header
		//

		Blam::Tags::Tag s_HeaderTag;
		s_MapFile.read(reinterpret_cast<char *>(&s_HeaderTag), sizeof(Blam::Tags::Tag));

		if (s_HeaderTag != 'head')
			return -1;

		//
		// Read the map's ID
		//

		int32_t s_MapID;
		s_MapFile.seekg(0x2DEC);
		s_MapFile.read(reinterpret_cast<char*>(&s_MapID), sizeof(int32_t));

		return s_MapID;
	}

	const auto Game_LoadMap = reinterpret_cast<bool(*)(uint8_t *, void *)>(0xA83AF0);

	bool GameImpl::LoadMap(const std::string &p_MapName)
	{
		auto s_LobbyType = GetLobbyType();

		if (s_LobbyType != GameLobbyType::CustomGame && s_LobbyType != GameLobbyType::Forge)
		{
			WriteLog("ERROR: You can only load a map from a Custom Games or Forge lobby!");
			return false;
		}

		//
		// Check to see if a map variant exists
		//

		auto s_MapPath = "mods\\maps\\" + p_MapName + "\\sandbox.map";

		uint8_t s_MapData[0xE090];
		std::ifstream s_MapVariant(s_MapPath, std::ios::binary);
		if (s_MapVariant.is_open())
		{
			WriteLog("Loading map variant '%s'...", s_MapPath.c_str());
			if (!LoadMapVariant(s_MapVariant, s_MapData))
			{
				WriteLog("ERROR: Invalid map variant file: %s", s_MapPath.c_str());
				return false;
			}
		}
		else
		{
			s_MapPath = "maps\\" + p_MapName + ".map";

			WriteLog("Loading default map '%s'...", s_MapPath.c_str());
			if (!LoadDefaultMapVariant(p_MapName, s_MapData))
			{
				WriteLog("ERROR: Invalid map file: %s", s_MapPath.c_str());
				return false;
			}
		}

		if (!Game_LoadMap(s_MapData, nullptr))
		{
			WriteLog("ERROR: Failed to load map: %s", s_MapPath.c_str());
			return false;
		}

		SaveMapVariantToPreferences(s_MapData);

		WriteLog("Map '%s' loaded successfully!", p_MapName.c_str());
		return true;
	}

	bool GameImpl::OnMapLoaded(void *p_Data)
	{
		// TODO: Do stuff after a map was loaded

		return true;
	}

	const auto InitializeMapVariant = reinterpret_cast<void(__thiscall *)(uint8_t *, int32_t)>(0x581F70);
	const auto ParseMapVariant = reinterpret_cast<bool(__thiscall *)(void *, uint8_t *, bool *)>(0x573250);

	bool GameImpl::LoadMapVariant(std::ifstream &p_File, uint8_t *out)
	{
		if (!p_File.is_open())
			return false;

		//
		// Verify the size of the map variant file
		//

		p_File.seekg(0, std::ios::end);
		auto s_FileSize = p_File.tellg();
		p_File.seekg(0, std::ios::beg);

		if (s_FileSize < 0xE1F0)
			return false;

		// Load it into a buffer and have the game parse it
		uint8_t s_BLFData[0xE1F0];
		p_File.read(reinterpret_cast<char*>(s_BLFData), 0xE1F0);

		return ParseMapVariant(s_BLFData, out, nullptr);
	}

	bool GameImpl::LoadDefaultMapVariant(const std::string &p_MapName, uint8_t *out)
	{
		int s_MapID = GetMapID(p_MapName);

		if (s_MapID < 0)
			return false;

		InitializeMapVariant(out, s_MapID);

		// Make sure it actually loaded the map correctly by verifying that the
		// variant is valid for the map
		int32_t firstMapId = *reinterpret_cast<int32_t*>(out + 0xE0);
		int32_t secondMapId = *reinterpret_cast<int32_t*>(out + 0x100);
		return (firstMapId == s_MapID && secondMapId == s_MapID);
	}

	void GameImpl::SaveMapVariantToPreferences(const uint8_t *data)
	{
		size_t s_VariantOffset;

		switch (GetLobbyType())
		{
		case GameLobbyType::CustomGame:
			s_VariantOffset = 0x7F0;
			break;

		case GameLobbyType::Forge:
			s_VariantOffset = 0xEA98;
			break;

		default:
			return;
		}

		// Copy the data in
		auto s_SavedVariant = reinterpret_cast<uint8_t *>(0x22C0130 + s_VariantOffset);
		memcpy(s_SavedVariant, data, 0xE090);

		// Mark preferences as dirty
		*reinterpret_cast<bool *>(0x22C0129) = true;
	}

	GameLobbyType GameImpl::GetLobbyType() const
	{
		return reinterpret_cast<GameLobbyType(__thiscall *)()>(0x435640)();
	}

	const std::vector<std::string> &GameImpl::GetMaps() const
	{
		return m_Maps;
	}

	const std::vector<std::string> &GameImpl::GetMapVariants() const
	{
		return m_MapVariants;
	}
}