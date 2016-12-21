#pragma once
#include "Interfaces\IInitializable.hpp"
#include "Utils\Singleton.hpp"

namespace AnvilEldorado
{
	class Forge final : public AnvilCommon::IInitializable, public AnvilCommon::Singleton<Forge>
	{
	public:
		bool Init() override;
		
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