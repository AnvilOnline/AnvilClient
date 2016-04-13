#pragma once
#include <string>
#include <list>
#include "SettingsEntry.hpp"

namespace Anvil
{
	namespace Client
	{
		namespace Settings
		{
			class SettingsGroup
			{
				std::string m_Name;
				std::string m_Description;

				std::list<SettingsEntry> m_Settings;

			public:
				SettingsGroup(std::string p_Name, std::string p_Description);

				std::string& GetName();

				std::string& GetDescription();

				bool AddSetting(SettingsEntry p_Entry);

				bool GetSetting(std::string p_Name, SettingsEntry& p_Entry);

				bool RemoveSetting(std::string p_Name);

				bool Contains(std::string p_Name);
			};
		}
	}
}