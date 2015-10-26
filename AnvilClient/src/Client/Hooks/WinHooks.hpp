#pragma once
#include <Misc/IInit.hpp>
#include <Misc/Macros.hpp>
#include <d3dx9.h>

namespace Anvil
{
	namespace Client
	{
		namespace Hooks
		{
			class WinHooks : public IInit
			{
				void* m_MouseHook;
				void* m_WindowHook;

				DeclareFunction(HWND, CreateWindowExA, __stdcall, DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hwndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
				DeclareFunction(HRESULT, D3DDevice9_EndScene, __stdcall, LPDIRECT3DDEVICE9 p_Device);
				DeclareFunction(HRESULT, D3DDevice9_Reset, __stdcall, LPDIRECT3DDEVICE9 p_Device, D3DPRESENT_PARAMETERS* p_PresentationParameters);
				DeclareFunction(HRESULT, D3DDevice9_BeginScene, __stdcall, LPDIRECT3DDEVICE9 p_Device);
				DeclareFunction(void*, Direct3DCreate9, __stdcall, unsigned int p_SdkVersion);

				void Hook_DirectX();

			protected:
				~WinHooks()
				{
				}

			public:
				WinHooks();
				bool Init() override;
			};
		}
	}
}