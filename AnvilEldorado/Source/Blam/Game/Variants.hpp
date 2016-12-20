#pragma once
#include <cstdint>

namespace Blam::Game
{
	enum class GameType : int32_t
	{
		None,
		CTF,
		Slayer,
		Oddball,
		KOTH,
		Forge,
		VIP,
		Juggernaut,
		Territories,
		Assault,
		Infection,

		Count
	};

	enum class GameMode : int32_t
	{
		Invalid,
		Campaign,
		Multiplayer,
		Mainmenu,
		Shared,
		
		Count
	};
	
	//
	// TODO: Find a better spot for the structures below
	//

	struct GameContentHeader
	{
		uint64_t ID; // not sure about this
		wchar_t Name[0x10];
		char Description[0x80];
		char Author[0x10];
		uint32_t Type;
		uint32_t Unknown1; // 0
		uint32_t Unknown2; // 1
		uint32_t Unknown3; // 0
		uint32_t Size; // 0xE1F0 for maps, 0x3BC for variants
		uint32_t Unknown4; // 0
		uint32_t UnknownUnique1; // changes for each item
		uint32_t Unknown5; // 0
		uint32_t Unknown6; // 0
		uint32_t Unknown7; // -1
		uint32_t Unknown8; // 0x140 for maps, -1 for variants, 
		uint64_t Unknown9; // 0 for maps, 4 for variants
		uint32_t Unknown10; // -1;
		uint32_t Unknown11; // 0;
		uint32_t Unknown12; // 0;
		uint32_t Unknown13; // 0;
	};

	struct GameContentItem
	{
		uint32_t Index;
		uint32_t ItemFlags;
		uint32_t ItemType;
		uint32_t BaseOffset;
		GameContentHeader ContentHeader;
		wchar_t FilePath[0x99];
	};

	struct GameVariant
	{
		GameType Engine : 32;
		float Unknown1;
		uint8_t Unknown2[0x2C];
		wchar_t Name[0x10];
		char Description[0x80];
		char Author[0x10];
		uint8_t Unknown3[0x40];
		uint8_t TeamGame;
		uint8_t RoundTimeLimit;
		uint8_t NumberOfRounds;
		uint8_t Unknown5;
		uint8_t Unknown6;
		uint8_t Unknown7;
		uint8_t Unknown8;
		uint8_t Unknown9;
		/* 0xC58 */ uint8_t Unknown10; // default 0xA
		uint8_t RespawnTime;
		uint8_t Unknown11; // default 5
		uint8_t Unknown12; // 0
		uint8_t Unknown13; // default 5
		uint8_t Unknown14; // 0
		uint8_t Unknown15; // 0
		uint8_t Unknown16; // 0
		/* 0xC60 */ uint8_t Unknown17; // 0
		uint8_t Unknown18; // 0
		uint8_t Unknown19; // 0
		uint8_t Unknown20; // 0
		uint8_t Unknown21; // 0
		uint8_t Unknown22; // 0
		uint8_t Unknown23; // 0
		uint8_t Unknown24; // 0
		/* 0xC68 */ uint8_t Unknown25; // 0
		uint8_t Unknown26; // 0
		uint8_t Unknown27; // 0
		uint8_t Unknown28; // default 0xFE
		uint8_t Unknown29; // default 0xFE
		uint8_t Unknown30; // 0
		uint8_t Unknown31; // 0
		uint8_t Unknown32; // 0
		/* 0xC70 */ uint8_t Unknown33; // 0
		uint8_t Unknown34; // 0
		uint8_t Unknown35; // 0
		uint8_t Unknown36; // 0
		uint8_t Unknown37; // 0
		uint8_t Unknown38; // 0
		uint8_t Unknown39; // 0
		uint8_t Unknown40; // 0
		/* 0xC78 */ uint8_t Unknown41; // 0
		uint8_t Unknown42; // 0
		uint8_t Unknown43; // 0
		uint8_t Unknown44; // 0
		uint8_t Unknown45; // 0
		uint8_t Unknown46; // default 7
		uint8_t Unknown47; // 0
		uint8_t Unknown48; // default 1
		/* 0xC80 */ uint8_t Unknown49; // 0
		uint8_t Unknown50; // default 1
		uint8_t Unknown51; // 0
		uint8_t Unknown52; // 0
		uint8_t Unknown53; // 0
		uint8_t Unknown54; // 0
		uint8_t Unknown55; // 0
		uint8_t Unknown56; // 0
		/* 0xC88 */ uint8_t Unknown57; // 0
		uint8_t Unknown58; // 0
		uint8_t Unknown59; // 0
		uint8_t Unknown60; // 0
		uint8_t Unknown61; // 0
		uint8_t Unknown62; // 0
		uint8_t Unknown63; // 0
		uint8_t Unknown64; // default 0xFE
		uint8_t Unknown65;
	};
}