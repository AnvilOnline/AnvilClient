#pragma once
#define _WIN32_MEAN_AND_LEAN

#include <Interfaces\Engine.hpp>
#include <Hooking\Hooking.hpp>

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