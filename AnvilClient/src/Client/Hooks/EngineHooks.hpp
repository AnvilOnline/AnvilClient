#pragma once
#include <Misc/IInit.hpp>
#include <Misc/Macros.hpp>

namespace Anvil
{
	namespace Client
	{
		namespace Hooks
		{
			class EngineHooks : public IInit
			{
			protected:
				~EngineHooks()
				{
				}

			private:
				DeclareFunction(int, GameTick, __stdcall);
				DeclareFunction(bool, VerifyMapSignature, __fastcall, void* This);
				DeclareFunction(void, StartCountdownTimer, __stdcall);

				DeclareFunction(void*, sub_5C7E40, __stdcall);

			public:
				bool Init() override;
			};
		}
	}
}