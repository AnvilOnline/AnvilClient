#pragma once
#include <Misc/IInit.hpp>
#include <Misc/Macros.hpp>
#include <d3d9.h>

namespace Anvil
{
	namespace Client
	{
		namespace Hooks
		{
			class WinHooks : public IInit
			{
				static HHOOK m_MouseHook;
				static HHOOK m_WindowHook;

				static LRESULT __stdcall HookMouseProc(int p_Code, WPARAM p_WParam, LPARAM p_LParam);
				static LRESULT __stdcall HookWindowProc(int p_Code, WPARAM p_WParam, LPARAM p_LParam);

				DeclareFunction(HWND, CreateWindowExA, __stdcall, DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hwndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
				DeclareFunction(HRESULT, D3DDevice9_EndScene, __stdcall, LPDIRECT3DDEVICE9 p_Device);
				DeclareFunction(HRESULT, D3DDevice9_Reset, __stdcall, LPDIRECT3DDEVICE9 p_Device, D3DPRESENT_PARAMETERS* p_PresentationParameters);
				DeclareFunction(HRESULT, D3DDevice9_BeginScene, __stdcall, LPDIRECT3DDEVICE9 p_Device);
				DeclareFunction(void*, Direct3DCreate9, __stdcall, uint32_t p_SdkVersion);
				DeclareFunction(HRESULT, Direct3D_CreateDevice, __stdcall, void* Th, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface);

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