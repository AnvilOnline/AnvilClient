#pragma once
#include <memory>
#include <mutex>
#include <fstream>
#include <Interfaces\Initializable.hpp>

#define WriteLog(p_Format, ...) AnvilCommon::Utils::Logger::GetInstance()->InternalWriteLog(__FUNCTION__, __LINE__, p_Format, __VA_ARGS__);

namespace AnvilCommon::Utils
{
	class Logger : 
		public Initializable
	{
	private:
		std::mutex m_Lock;
		std::ofstream m_Stream;
		void* m_ConsoleHandle;
		bool m_Debug;

	public:
		static std::shared_ptr<Logger> GetInstance();

		virtual bool Init();

		bool InitConsole(std::string p_Title);
		bool InternalWriteLog(char* p_Function, int32_t p_line, char* p_Format, ...);
	};
}