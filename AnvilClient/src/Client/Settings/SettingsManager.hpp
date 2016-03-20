#pragma once
#include <string>
#include <memory>
#include <list>

namespace Anvil
{
	namespace Client
	{
		namespace Settings
		{
			class SettingsManager
			{
				template <class T>
				class SettingsObject
				{
					std::shared_ptr<T> m_Object;
					std::string m_Name;
					std::string m_Description;

				public:
					SettingsObject(std::string p_Name, std::string p_Description);

					SettingsObject(std::string p_Name, std::string p_Description, T* p_Object);
				};

				std::list<SettingsObject<void>> m_Settings;
			};
		}
	}
}