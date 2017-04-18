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