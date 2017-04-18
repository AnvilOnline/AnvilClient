#include "NetworkingImpl.hpp"
#include "Hooks.hpp"

using namespace AnvilEldorado::Game::Networking;

bool NetworkingImpl::Init()
{
	m_Hooks = std::make_shared<Hooks>();
	if (!m_Hooks->Init())
		return false;

	return true;
}
