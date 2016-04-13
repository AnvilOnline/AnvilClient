#include "SettingsManager.hpp"
#include "SettingsGroup.hpp"
#include <boost/algorithm/string.hpp>
#include <Utils/Logger.hpp>

using namespace Anvil::Client::Settings;


bool SettingsManager::Init()
{
	// Add default groups
	auto s_Added = AddSettingsGroup("anvil", "General settings for the anvil client.");
	if (!s_Added)
		WriteLog("Could not create anvil settings group.");

	s_Added = AddSettingsGroup("player", "Player related settings and information");
	if (!s_Added)
		WriteLog("Could not create player settings group.");

	return true;
}

bool SettingsManager::AddSettingsGroup(std::string p_Name, std::string p_Description)
{
	if (p_Name.empty())
		return false;

	for (auto& l_Group : m_Groups)
	{
		if (boost::iequals(p_Name, l_Group.GetName()))
			return false;
	}

	m_Groups.push_back(SettingsGroup(p_Name, p_Description));

	return true;
}

std::list<SettingsGroup>& SettingsManager::GetGroups()
{
	return m_Groups;
}

SettingsGroup& SettingsManager::GetGroup(std::string p_Name)
{
	for (auto& l_Group : m_Groups)
	{
		if (boost::iequals(l_Group.GetName(), p_Name))
			return l_Group;
	}

	return SettingsGroup("invalid", "invalid");
}
