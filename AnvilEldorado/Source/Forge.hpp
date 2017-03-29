#pragma once
#include <Interfaces\Initializable.hpp>

namespace AnvilEldorado
{
	class Forge : 
		public AnvilCommon::Initializable
	{
	public:
		virtual bool Init();
		
		void UpdateBarriersEnabled();

		bool ShouldDelete() const;
		void SetShouldDelete(const bool p_ShouldDelete = true);

		bool BarriersEnabledValid() const;
		void SetBarriersEnabledValid(const bool p_Valid = true);

		bool KillBarriersEnabled() const;
		void SetKillBarriersEnabled(const bool p_Enabled = true);

		bool PushBarriersEnabled() const;
		void SetPushBarriersEnabled(const bool p_Enabled = true);

	private:
		bool m_BarriersEnabledValid = false;
		bool m_KillBarriersEnabled = true;
		bool m_PushBarriersEnabled = true;
	};
}