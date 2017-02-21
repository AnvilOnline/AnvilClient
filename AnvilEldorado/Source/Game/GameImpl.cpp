#include "GameImpl.hpp"
#include "Hooks.hpp"

using namespace AnvilEldorado::Game;

bool GameImpl::Init()
{
	// Initialize game hooks
	m_Hooks = std::make_shared<Hooks>();
	if (!m_Hooks->Init())
		WriteLog("Could not initialize GameImpl::Hooks.");

	return true;
}