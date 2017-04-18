#pragma once
#include <Interfaces\Initializable.hpp>

#include <Blam\Text\StringID.hpp>

#include <memory>

namespace AnvilEldorado::Game::UI
{
	class UIImpl :
		public AnvilCommon::Initializable
	{
	protected:
		std::shared_ptr<AnvilCommon::Initializable> m_Hooks;

	public:
		virtual bool Init();

		static void* ShowDialog(const Blam::Text::StringID &p_DialogID, const int32_t p_Arg1, const int32_t p_Flags, const Blam::Text::StringID &p_ParentID);
	};
}