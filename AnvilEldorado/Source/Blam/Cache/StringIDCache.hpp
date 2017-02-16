//#pragma once
//#include <cstdint>
//#include <string>
//#include "Blam\Text\StringID.hpp"
//
//namespace Blam::Cache
//{
//	struct StringIDCacheHeader
//	{
//		int32_t StringCount;
//		int32_t StringDataSize;
//	};
//
//	struct StringIDCache : public AnvilCommon::Singleton<StringIDCache>
//	{
//		StringIDCacheHeader Header;
//		char *Data;
//		char **Strings;
//
//		StringIDCache();
//		~StringIDCache();
//
//		bool Load(const std::string &path);
//		char *GetString(const Text::StringID &stringID);
//	};
//}