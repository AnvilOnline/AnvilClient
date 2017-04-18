#include "AudioImpl.hpp"
#include "Hooks.hpp"

using namespace AnvilEldorado::Game::Audio;

bool AudioImpl::Init()
{
	m_Hooks = std::make_shared<Hooks>();
	if (!m_Hooks->Init())
		return false;

	return true;
}
