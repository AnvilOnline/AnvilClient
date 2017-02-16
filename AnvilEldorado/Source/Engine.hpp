#pragma once
#define _WIN32_MEAN_AND_LEAN

#include <Interfaces/IEngineInitializable.hpp>

#include <memory>

#include <Windows.h>

namespace AnvilEldorado
{
	class Engine : 
		public AnvilCommon::IEngineInitializable
	{
	private:
		void* m_ModuleBase;
		size_t m_ModuleSize;

	protected:
		std::shared_ptr<AnvilCommon::IInitializable> m_Audio;
		std::shared_ptr<AnvilCommon::IInitializable> m_Camera;
		std::shared_ptr<AnvilCommon::IInitializable> m_Forge;
		std::shared_ptr<AnvilCommon::IInitializable> m_Game;
		std::shared_ptr<AnvilCommon::IInitializable> m_Graphics;
		std::shared_ptr<AnvilCommon::IInitializable> m_Input;
		std::shared_ptr<AnvilCommon::IInitializable> m_Network;
		std::shared_ptr<AnvilCommon::IInitializable> m_Player;
		std::shared_ptr<AnvilCommon::IInitializable> m_UI;

		/// <summary>
		/// Create Window Hook
		/// </summary>
		typedef HWND(__stdcall* CreateWindowExA_t)(DWORD, LPCSTR, LPCSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);
		static CreateWindowExA_t o_CreateWindowExA;
		static HWND __stdcall hk_CreateWindowExA(DWORD, LPCSTR, LPCSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);

		/// <summary>
		/// Bink Video Hook
		/// </summary>
		typedef bool(__cdecl* LoadBinkVideo_t)(void*, void*);
		static LoadBinkVideo_t o_LoadBinkVideo;
		static bool __cdecl hk_LoadBinkVideo(void*, void*);

		/// <summary>
		/// Saber Halo 3 initialization
		/// </summary>
		typedef void* (*Game_InitHalo3_t)();
		static Game_InitHalo3_t o_Game_InitHalo3;
		static void* hk_Game_InitHalo3();

	public:
		Engine();
		~Engine();

		virtual bool Init() override;
		virtual uint8_t* ExecutableBase() override;
		virtual size_t ExecutableSize() override;

	private:
		void CreateHooks();
	};
}