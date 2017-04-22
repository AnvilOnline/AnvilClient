#pragma once
#define _WIN32_MEAN_AND_LEAN

#include <Interfaces\Engine.hpp>
#include <Interfaces\IdCache.hpp>
#include <Hooking\Hooking.hpp>

#include <memory>

#include <Windows.h>

namespace AnvilEldorado
{
	class EngineImpl :
		public AnvilCommon::Engine
	{
	private:
		std::shared_ptr<AnvilCommon::IdCache> m_Cache;
		void* m_ModuleBase;
		size_t m_ModuleSize;

	protected:
		std::shared_ptr<AnvilCommon::Initializable> m_Audio;
		std::shared_ptr<AnvilCommon::Initializable> m_Camera;
		std::shared_ptr<AnvilCommon::Initializable> m_Forge;
		std::shared_ptr<AnvilCommon::Initializable> m_Game;
		std::shared_ptr<AnvilCommon::Initializable> m_Graphics;
		std::shared_ptr<AnvilCommon::Initializable> m_Input;
		std::shared_ptr<AnvilCommon::Initializable> m_Networking;
		std::shared_ptr<AnvilCommon::Initializable> m_Player;
		std::shared_ptr<AnvilCommon::Initializable> m_UI;

		/// <summary>
		/// Create Window Hook
		/// </summary>
		DeclareFunction(HWND, __stdcall, CreateWindowExA, DWORD p_ExStyle, LPCSTR p_ClassName, LPCSTR p_WindowName, DWORD p_Style, int p_X, int p_Y, int p_Width, int p_Height, HWND p_Parent, HMENU p_Menu, HINSTANCE p_Instance, LPVOID p_Param);

		/// <summary>
		/// Bink Video Hook
		/// </summary>
		DeclareFunction(bool, __cdecl, LoadBinkVideo, int p_VideoID, char *p_DestBuf);

		/// <summary>
		/// Tag Cache Validation Hook
		/// </summary>
		DeclareFunction(char, __cdecl, ValidateTagCache, void *a1);

		/// <summary>
		/// Account Processing Hook
		/// </summary>
		DeclareFunction(char*, __cdecl, ProcessAccountInfo);

		/// <summary>
		/// Account Verification Hook
		/// </summary>
		DeclareFunction(char, __cdecl, VerifyAccountAndLoadAnticheat);

		/// <summary>
		/// Force Russian Localization Hook
		/// </summary>
		DeclareFunction(char, __cdecl, ForceRussianLocale);

	public:
		EngineImpl();
		~EngineImpl();

		virtual bool Init();
		virtual uint8_t* ExecutableBase();
		virtual size_t ExecutableSize();
		virtual std::shared_ptr<AnvilCommon::IdCache> GetStringCache();

	private:
		void CreateHooks();
	};
}