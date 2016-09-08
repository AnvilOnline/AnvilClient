#include "SettingsEntry.hpp"

using namespace Anvil::Client::Settings;

SettingsEntry::SettingsEntry(std::wstring p_Name, std::wstring p_Description, std::wstring p_DefaultValue)
	: m_Name(p_Name),
	m_Description(p_Description),
	m_Value(p_DefaultValue),
	m_DefaultValue(p_DefaultValue)
{
}

auto& SettingsEntry::GetName()
{
	return m_Name;
}

auto& SettingsEntry::GetDescription()
{
	return m_Description;
}

auto& SettingsEntry::GetValue()
{
	return m_Value;
}

void SettingsEntry::SetValue(std::wstring p_Value)
{
	m_Value = p_Value;
}

void SettingsEntry::Reset()
{
	m_Value = m_DefaultValue;
}

