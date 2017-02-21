#pragma once
#include <Interfaces\Initializable.hpp>
#include <memory>

namespace AnvilEldorado::Audio
{
	class AudioImpl
		: public AnvilCommon::Initializable
	{
	protected:
		std::shared_ptr<AnvilCommon::Initializable> m_Hooks;

	public:
		virtual bool Init();
	};
}