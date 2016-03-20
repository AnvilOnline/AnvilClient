#include "SettingsManager.hpp"

using namespace Anvil::Client::Settings;

template <class T>
SettingsManager::SettingsObject<T>::SettingsObject(std::string p_Name, std::string p_Description)
	: m_Name(p_Name),
	m_Description(p_Description)
{
	m_Object = std::make_shared<T>();
}

template <class T>
SettingsManager::SettingsObject<T>::SettingsObject(std::string p_Name, std::string p_Description, T* p_Object)
	: m_Object(p_Object),
	m_Name(p_Name),
	m_Description(p_Description)
{
}