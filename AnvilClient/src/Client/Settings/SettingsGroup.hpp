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
				std::wstring m_Name;
				std::wstring m_Description;

				std::list<SettingsEntry> m_Settings;

			public:
				SettingsGroup(std::wstring p_Name, std::wstring p_Description);

				std::wstring& GetName();

				std::wstring& GetDescription();

				bool AddSetting(SettingsEntry p_Entry);

				bool GetSetting(std::wstring p_Name, SettingsEntry& p_Entry);

				bool RemoveSetting(std::wstring p_Name);

				bool Contains(std::wstring p_Name);
			};
		}
	}
}