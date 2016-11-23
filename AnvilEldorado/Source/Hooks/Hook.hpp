#pragma once
#include <Interfaces/IInitializable.hpp>
#include <cstdint>

namespace AnvilEldorado::Hooks
{
	void Armor_AfterTagsLoaded();
	void Armor_RefreshUiPlayer();

	class Hook : AnvilCommon::IInitializable
	{
	private:
		typedef uint32_t(__stdcall* EndScene_t)(void* p_Device);
		static EndScene_t EndScene;
		static uint32_t __stdcall c_EndScene(void* p_Device);

	private:
		void Hook_DirectX();
		void Hook_Armor();

	public:
		virtual bool Init() override;
	};
}