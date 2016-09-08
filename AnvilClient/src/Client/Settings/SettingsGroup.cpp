#include "SettingsGroup.hpp"

using namespace Anvil::Client::Settings;

SettingsGroup::SettingsGroup(std::wstring p_Name, std::wstring p_Description)
	: m_Name(p_Name),
	m_Description(p_Description)
{
}

std::wstring& SettingsGroup::GetName()
{
	return m_Name;
}

std::wstring& SettingsGroup::GetDescription()
{
	return m_Description;
}

bool SettingsGroup::AddSetting(SettingsEntry p_Entry)
{
	return false;
}

bool SettingsGroup::GetSetting(std::wstring p_Name, SettingsEntry & p_Entry)
{
	return false;
}

bool SettingsGroup::RemoveSetting(std::wstring p_Name)
{
	return false;
}

bool SettingsGroup::Contains(std::wstring p_Name)
{
	return false;
}
