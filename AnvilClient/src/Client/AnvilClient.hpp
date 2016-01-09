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
			uint16_t* m_MapResetBit;

			std::string m_Version;

			AnvilClient();

		protected:
			~AnvilClient();

		public:
			static AnvilClient* GetInstance();
			bool Init() override;
			bool PreInit() override;
			bool PostInit() override;
			
			void* GetMapInfoBlock();
			bool ForceLoadMap(std::string p_MapName, int32_t p_GameEngineMode, int32_t p_GameType);

			std::string GetVersion();
		};
	}
}