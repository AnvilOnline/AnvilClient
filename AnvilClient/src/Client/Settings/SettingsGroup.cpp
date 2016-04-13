#include "SettingsGroup.hpp"

using namespace Anvil::Client::Settings;

SettingsGroup::SettingsGroup(std::string p_Name, std::string p_Description)
	: m_Name(p_Name),
	m_Description(p_Description)
{
}

std::string& SettingsGroup::GetName()
{
	return m_Name;
}

std::string& SettingsGroup::GetDescription()
{
	return m_Description;
}

bool SettingsGroup::AddSetting(SettingsEntry p_Entry)
{
	return false;
}

bool SettingsGroup::GetSetting(std::string p_Name, SettingsEntry & p_Entry)
{
	return false;
}

bool SettingsGroup::RemoveSetting(std::string p_Name)
{
	return false;
}

bool SettingsGroup::Contains(std::string p_Name)
{
	return false;
}
