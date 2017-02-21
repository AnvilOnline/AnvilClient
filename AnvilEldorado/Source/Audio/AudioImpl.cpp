#include "AudioImpl.hpp"
#include "Hooks.hpp"

using namespace AnvilEldorado::Audio;

bool AudioImpl::Init()
{
	m_Hooks = std::make_shared<AnvilEldorado::Audio::Hooks>();
	if (!m_Hooks->Init())
		WriteLog("Could not initialize AudioImpl::Hooks.");

	return true;
}