#pragma once
#include <string>

namespace Anvil
{
	namespace Client
	{
		namespace Settings
		{
			class SettingsEntry
			{
				std::wstring m_Name;
				std::wstring m_Description;
				std::wstring m_Value;
				std::wstring m_DefaultValue;

			public:
				SettingsEntry(std::wstring p_Name, std::wstring p_Description, std::wstring p_DefaultValue = L"");

				auto& GetName();
				auto& GetDescription();
				auto& GetValue();
				void SetValue(std::wstring p_Value);
				void Reset();
			};
		}
	}
}