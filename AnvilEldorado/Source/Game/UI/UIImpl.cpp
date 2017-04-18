#include "UIImpl.hpp"
#include "Hooks.hpp"

using namespace AnvilEldorado::Game::UI;

bool UIImpl::Init()
{
	m_Hooks = std::make_shared<UI::Hooks>();
	if (!m_Hooks->Init())
		return false;

	return true;
}

const auto UI_Alloc = reinterpret_cast<void *(__cdecl *)(int32_t)>(0xAB4ED0);
const auto UI_OpenDialogById = reinterpret_cast<void *(__thiscall *)(void *, Blam::Text::StringID, int32_t, int32_t, Blam::Text::StringID)>(0xA92780);
const auto UI_PostMessage = reinterpret_cast<int(*)(void *)>(0xA93450);

void* UIImpl::ShowDialog(const Blam::Text::StringID &p_DialogID, const int32_t p_Arg1, const int32_t p_Flags, const Blam::Text::StringID &p_ParentID)
{
	// TODO: Fix
	//WriteLog("Showing ui dialog '%s'...", Blam::Cache::StringIDCache::Instance()->GetString(p_DialogID));

	auto *s_UIData = UI_Alloc(0x40);

	if (!s_UIData)
		return nullptr;

	UI_OpenDialogById(s_UIData, p_DialogID, p_Arg1, p_Flags, p_ParentID);
	UI_PostMessage(s_UIData);

	return s_UIData;
}