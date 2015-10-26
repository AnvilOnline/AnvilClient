#pragma once
#include <Misc/IInit.hpp>

namespace Anvil
{
	namespace Client
	{
		class AnvilClient : IInit
		{
			static AnvilClient* m_Instance;

			IInit* m_WinHooks;
			IInit* m_EngineHooks;
			IInit* m_EnginePatches;

			AnvilClient();

		protected:
			~AnvilClient();

		public:
			static AnvilClient* GetInstance();
			bool Init() override;
			bool PreInit() override;
			bool PostInit() override;
			
		};
	}
}