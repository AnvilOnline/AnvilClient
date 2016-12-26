#pragma once
#include "Interfaces\IInitializable.hpp"
#include "Utils\Singleton.hpp"

namespace AnvilEldorado
{
	class Audio final : public AnvilCommon::IInitializable, public AnvilCommon::Singleton<Audio>
	{
	public:
		bool Init() override;
	};
}