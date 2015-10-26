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

				unsigned long m_Width, m_Height;

				WebRenderer();

			protected:
				~WebRenderer()
				{
				}

			public:
				bool Init() override;
			};
		}
	}
}