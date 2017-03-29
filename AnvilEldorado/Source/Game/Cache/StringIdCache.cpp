#include "StringIdCache.hpp"
#include <filesystem>
#include <fstream>

#include <Utils\Logger.hpp>

#include <Blam\Cache\StringIDCacheHeader.hpp>

using namespace AnvilEldorado::Game::Cache;

bool StringIdCache::Init()
{
	// Parse the string_id file if it exists
	if (!ParseStringIds("maps\\string_ids.dat"))
		return false;

	return true;
}

bool StringIdCache::ParseStringIds(const std::string p_Path)
{
	// Check if the file exists
	if (!std::experimental::filesystem::exists(p_Path))
		return false;

	// Open a stream to our file
	std::ifstream s_Stream(p_Path, std::ios::binary);

	// Check if our stream is open
	if (!s_Stream.is_open())
		return false;

	// Hold our header for parsing
	Blam::Cache::StringIDCacheHeader s_Header = { 0 };

	// Read out the header
	s_Stream.read(reinterpret_cast<char*>(&s_Header), sizeof(s_Header));

	// Create new data to hold our offsets
	auto s_Offsets = std::shared_ptr<uint32_t>(new uint32_t[s_Header.Count], std::default_delete<uint32_t[]>());
	// TODO: Determine if we need to set everything to -1?

	// Create memory to hold all of our strings
	auto s_Strings = std::shared_ptr<char>(new char[s_Header.Size + 1], std::default_delete<char[]>());
	std::memset(s_Strings.get(), 0, s_Header.Size + 1);

	// Read all of our offsets
	s_Stream.read(reinterpret_cast<char*>(s_Offsets.get()), sizeof(uint32_t) * s_Header.Count);

	// Read all of our strings
	s_Stream.read(reinterpret_cast<char*>(s_Strings.get()), s_Header.Size);

	// We are finished with the stream
	s_Stream.close();

	// Assign all of our strings to the map for future access
	for (auto i = 0; i < s_Header.Count; ++i)
	{
		auto l_Offset = s_Offsets.get()[i];

		// Ignore Blam "null" values, and out of range values
		if (l_Offset < 0 || l_Offset > s_Header.Size)
		{
			WriteLog("string_id out of range! index: %d offset: %d", i, l_Offset);
			continue;
		}

		// Add our string to the list
		m_Strings.insert_or_assign(i, std::string(s_Strings.get() + l_Offset));
	}

	// Some output logging
	WriteLog("Read %d string_id's from %s.", Count(), p_Path.c_str());

	// shared_ptr should free the rest of the memory we used
	return true;
}

std::string StringIdCache::GetString(uint16_t p_Id)
{
	// Bounds checking
	if (p_Id > Count())
		return std::string();

	// Return our string
	return m_Strings[p_Id];
}