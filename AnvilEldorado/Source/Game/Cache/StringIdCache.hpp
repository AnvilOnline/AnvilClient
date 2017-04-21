#pragma once
#include <Interfaces\IdCache.hpp>
#include <unordered_map>
#include <string>

namespace AnvilEldorado::Game::Cache
{
	class StringIdCache :
		public AnvilCommon::IdCache
	{
	protected:
		std::unordered_map<int16_t, std::string> m_Strings;

	private:
		bool ParseStringIds(const std::string p_Path);

	public:
		virtual bool Init();

		virtual bool Empty() { return m_Strings.empty(); }
		virtual int32_t Count() { return m_Strings.size(); }
		virtual std::string GetString(int16_t p_Id);
	};
}