#pragma once
#define _WIN32_MEAN_AND_LEAN

#include <Interfaces/Engine.hpp>
#include <Hooking/Hooking.hpp>

#include <memory>

#include <Windows.h>

namespace AnvilEldorado
{
	class EngineImpl :
		public AnvilCommon::Engine
	{
	private:
		void* m_ModuleBase;
		size_t m_ModuleSize;

	protected:
		std::shared_ptr<AnvilCommon::Initializable> m_Audio;
		std::shared_ptr<AnvilCommon::Initializable> m_Camera;
		std::shared_ptr<AnvilCommon::Initializable> m_Forge;
		std::shared_ptr<AnvilCommon::Initializable> m_Game;
		std::shared_ptr<AnvilCommon::Initializable> m_Graphics;
		std::shared_ptr<AnvilCommon::Initializable> m_Input;
		std::shared_ptr<AnvilCommon::Initializable> m_Network;
		std::shared_ptr<AnvilCommon::Initializable> m_Player;
		std::shared_ptr<AnvilCommon::Initializable> m_UI;

		/// <summary>
		/// Create Window Hook
		/// </summary>
		DeclareFunction(HWND, __stdcall, CreateWindowExA, DWORD, LPCSTR, LPCSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);

		/// <summary>
		/// Bink Video Hook
		/// </summary>
		DeclareFunction(bool, __cdecl, LoadBinkVideo, void*, void*);

	public:
		EngineImpl();
		~EngineImpl();

		virtual bool Init();
		virtual uint8_t* ExecutableBase();
		virtual size_t ExecutableSize();

	private:
		void CreateHooks();
	};
}