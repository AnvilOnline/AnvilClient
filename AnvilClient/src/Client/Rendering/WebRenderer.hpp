#pragma once
#include <Misc/IInit.hpp>
#include <d3d9.h>
#include <d3dx9.h>

namespace Anvil
{
	namespace Client
	{
		namespace Rendering
		{
			class WebRenderer : public IInit
			{
				LPD3DXSPRITE m_Sprite;
				LPDIRECT3DTEXTURE9 m_Texture;
				LPD3DXFONT m_Font;

				unsigned long m_Width, m_Height;

				static WebRenderer* m_Instance;
				WebRenderer();

			protected:
				~WebRenderer()
				{
				}

			public:
				static WebRenderer* GetInstance();

				bool Init() override;

				bool InitRenderer(LPDIRECT3DDEVICE9 p_Device);
				bool Render(LPDIRECT3DDEVICE9 p_Device);
			};
		}
	}
}