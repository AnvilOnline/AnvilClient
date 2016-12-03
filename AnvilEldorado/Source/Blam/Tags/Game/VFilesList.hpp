#pragma once
#include <cstdint>
#include <string>
#include "../TagBlock.hpp"
#include "../TagData.hpp"

namespace Blam::Tags::Game
{
	// Contains a list of vfiles
	struct VFilesList
	{
		// Contains information about a vfile
		struct VFileInfo
		{
			char Name[256];   // The name of the file (e.g. "hf2p_weapons_categories.ps")
			char Folder[256]; // The folder the file is located in (e.g. "ps\autogen\")
			int32_t Offset;   // The starting offset of the file from the file data block
			int32_t Size;     // The size of the file in bytes
		};

		TagBlock<VFileInfo> Files; // The files in the list
		TagData<uint8_t> Data;     // The data block containing the data for every file
	};
}