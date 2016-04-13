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
				std::string m_Name;
				std::string m_Description;
				std::string m_Value;
				std::string m_DefaultValue;

			public:
				SettingsEntry(std::string p_Name, std::string p_Description, std::string p_DefaultValue = "");

				auto& GetName();
				auto& GetDescription();
				auto& GetValue();
				void SetValue(std::string p_Value);
				void Reset();
			};
		}
	}
}