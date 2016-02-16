#pragma once
#include <Misc/IInit.hpp>
#include <mutex>
#include <fstream>
#include <memory>

#ifdef _DEBUG
#define WriteLog(p_Format, ...) Anvil::Utils::Logger::GetInstance()->InternalWriteLog(__FUNCTION__, __LINE__, p_Format, __VA_ARGS__);
#else
#pragma warning(disable : 4390)
#define WriteLog(p_Format, ...)
#endif

namespace Anvil
{
	namespace Utils
	{
		class Logger : IInit
		{
			std::mutex m_Lock;
			std::ofstream m_Stream;
			void* m_ConsoleHandle;

		public:
			static std::shared_ptr<Logger> GetInstance();
			bool Init() override;

			bool InitConsole(std::string p_Title);
			bool InternalWriteLog(char* p_Function, int32_t p_line, char* p_Format, ...);
		};
	}
}