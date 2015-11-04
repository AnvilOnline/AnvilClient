#pragma once
#include <Misc/IInit.hpp>
#include <mutex>
#include <fstream>

#ifdef _DEBUG
#define WriteLog(p_Format, ...) Anvil::Utils::Logger::GetInstance()->InternalWriteLog(__FUNCTION__, __LINE__, p_Format, __VA_ARGS__);
#else
#pragma warning(disable : 4390)
#define WriteLog(p_Format, ...) Anvil::Utils::Logger::GetInstance()->InternalWriteLog(__FUNCTION__, __LINE__, p_Format, __VA_ARGS__);
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

			static Logger* m_Instance;

		protected:
			~Logger()
			{
			}

		public:
			static Logger* GetInstance();
			bool Init() override;

			bool InitConsole(std::string p_title);
			bool InternalWriteLog(char* p_function, int p_line, char* p_format, ...);
		};
	}
}