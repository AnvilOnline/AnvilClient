#pragma once
#include <string>
#include <list>
#include <Misc/IInit.hpp>
#include "SettingsGroup.hpp"

namespace Anvil
{
	namespace Client
	{
		namespace Settings
		{

			class SettingsManager : public IInit
			{
				std::list<SettingsGroup> m_Groups;

			public:
				bool Init() override;

				bool AddSettingsGroup(std::wstring p_Name, std::wstring p_Description);

				std::list<SettingsGroup>& GetGroups();

				SettingsGroup& GetGroup(std::wstring p_Name);
			};
		}
	}
}