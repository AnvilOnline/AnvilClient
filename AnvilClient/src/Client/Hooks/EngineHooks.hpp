#pragma once
#include <Misc/IInit.hpp>
#include <Misc/Macros.hpp>

namespace Anvil
{
	namespace Client
	{
		namespace Hooks
		{
			class ThisDummy;

			class EngineHooks : public IInit
			{
				DeclareFunction(int, GameTick, __stdcall);
				DeclareFunction(bool, VerifyMapSignature, __fastcall, void* This);
				DeclareFunction(void, StartCountdownTimer, __stdcall);

				DeclareFunction(int, PrintDebug, __cdecl, char* p_Source, char* p_Format, ...);

			public:
				bool Init() override;
			};
		}
	}
}