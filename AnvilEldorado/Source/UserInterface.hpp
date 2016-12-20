#pragma once
#include "Interfaces\IInitializable.hpp"
#include "Utils\Singleton.hpp"
#include "Blam\Text\StringID.hpp"

namespace AnvilEldorado
{
	class UserInterface final : public AnvilCommon::IInitializable, public AnvilCommon::Singleton<UserInterface>
	{
	public:
		bool Init() override;

		bool ShowDialog(const Blam::Text::StringID &p_DialogID, const int32_t p_Arg1 = 0, const int32_t p_Flags = 4, const Blam::Text::StringID &p_ParentID = Blam::Text::StringID::Null);

	private:
		bool m_ShowDialog;
		Blam::Text::StringID m_DialogID = Blam::Text::StringID::Null;
		int32_t m_DialogArg1 = 0;
		int32_t m_DialogFlags = 4;
		Blam::Text::StringID m_DialogParentID = Blam::Text::StringID::Null;
		void *m_DialogData = nullptr;
	};
}