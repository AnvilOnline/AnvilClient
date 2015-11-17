#pragma once
#include <Misc/IInit.hpp>
#include <string>

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

			void* m_MapInfoBlock;
			short* m_MapResetBit;

			AnvilClient();

		protected:
			~AnvilClient();

		public:
			static AnvilClient* GetInstance();
			bool Init() override;
			bool PreInit() override;
			bool PostInit() override;
			
			void* GetMapInfoBlock();
			bool ForceLoadMap(std::string p_MapName, int p_GameEngineMode, int p_GameType);
		};
	}
}