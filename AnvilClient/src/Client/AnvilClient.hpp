#pragma once
#include <Misc/IInit.hpp>
#include <string>
#include <memory>

namespace Anvil
{
	namespace Client
	{
		class AnvilClient : IInit
		{
			std::unique_ptr<IInit> m_WinHooks;
			std::unique_ptr<IInit> m_EngineHooks;
			std::unique_ptr<IInit> m_EnginePatches;
			std::unique_ptr<IInit> m_SDKFunctions;

			void* m_MapInfoBlock;
			uint16_t* m_MapResetBit;

			std::string m_Version;
			volatile bool m_RenderingEnabled;

			
		public:
			AnvilClient();
			static std::shared_ptr<AnvilClient> GetInstance();
			bool Init() override;
			bool PreInit() override;
			bool PostInit() override;
			
			void* GetMapInfoBlock();
			bool ForceLoadMap(std::string p_MapName, int32_t p_GameEngineMode, int32_t p_GameType);
			bool IsRenderingEnabled();

			std::string GetVersion();

			bool Shutdown();
		};
	}
}