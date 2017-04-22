#pragma once
#include <Interfaces\Initializable.hpp>
#include <Hooking\Hooking.hpp>

struct XNADDR;
struct XNKID;

namespace AnvilEldorado::Game::Networking
{
	class Hooks :
		public AnvilCommon::Initializable
	{
	protected:
		DeclareFunction(char*, __cdecl, GetIPStringFromInAddr, XNADDR* p_InAddr);

		DeclareFunction(bool, __cdecl, XnAddrToInAddr, XNADDR *p_XnAddr, XNKID *p_XnKid, void *p_InAddr);
	public:
		virtual bool Init();
	};
}