#include "SettingsManager.hpp"
#include "SettingsGroup.hpp"
#include <Utils/Logger.hpp>

using namespace Anvil::Client::Settings;


bool SettingsManager::Init()
{
	// Add default groups
	auto s_Added = AddSettingsGroup(L"anvil", L"General settings for the anvil client.");
	if (!s_Added)
		WriteLog(L"Could not create anvil settings group.");

	s_Added = AddSettingsGroup(L"player", L"Player related settings and information");
	if (!s_Added)
		WriteLog(L"Could not create player settings group.");

	WriteLog(L"SettingsManager init.");
	return true;
}

bool SettingsManager::AddSettingsGroup(std::wstring p_Name, std::wstring p_Description)
{
	if (p_Name.empty())
		return false;

	for (auto& l_Group : m_Groups)
	{
		if (!_wcsicmp(p_Name.c_str(), l_Group.GetName().c_str()))
			return false;
	}

	m_Groups.push_back(SettingsGroup(p_Name, p_Description));

	return true;
}

std::list<SettingsGroup>& SettingsManager::GetGroups()
{
	return m_Groups;
}

SettingsGroup& SettingsManager::GetGroup(std::wstring p_Name)
{
	for (auto& l_Group : m_Groups)
	{
		if (!_wcsicmp(p_Name.c_str(), l_Group.GetName().c_str()))
			return l_Group;
	}

	return SettingsGroup(L"invalid", L"invalid");
}
