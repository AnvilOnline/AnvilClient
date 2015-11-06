#include <Client/Hooks/EngineHooks.hpp>
#include <Client/Rendering/WebRenderer.hpp>

using namespace Anvil::Client::Hooks;

HookedFunction(EngineHooks, int, GameTick, __stdcall)
{
	auto s_Ret = o_GameTick();

	__asm NOP;
	//Rendering::WebRenderer::GetInstance()->Update();
	return s_Ret;
}