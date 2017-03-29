#pragma once
#include <Interfaces\Initializable.hpp>
#include <unordered_map>
#include <string>

namespace AnvilEldorado::Game::Cache
{
	class StringIdCache :
		public AnvilCommon::Initializable
	{
	protected:
		std::unordered_map<uint16_t, std::string> m_Strings;

	private:
		bool ParseStringIds(const std::string p_Path);

	public:
		virtual bool Init();

		bool Empty() { return m_Strings.empty(); }
		uint32_t Count() { return m_Strings.size(); }
		std::string GetString(uint16_t p_Id);
	};
}