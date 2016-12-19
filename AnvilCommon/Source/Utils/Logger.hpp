#pragma once
#include <memory>
#include <mutex>
#include <fstream>
#include "Interfaces\IInitializable.hpp"
#include "Utils\Singleton.hpp"

#ifdef _DEBUG
#define WriteLog(p_Format, ...) AnvilCommon::Utils::Logger::Instance()->InternalWriteLog(__FUNCTION__, __LINE__, p_Format, __VA_ARGS__);
#else
#pragma warning(disable : 4390)
#define WriteLog(p_Format, ...)
#endif

namespace AnvilCommon::Utils
{
	class Logger : IInitializable, public AnvilCommon::Singleton<Logger>
	{
	private:
		std::mutex m_Lock;
		std::ofstream m_Stream;
		void* m_ConsoleHandle;
		bool m_Debug;

	public:
		bool Init() override;

		bool InitConsole(std::string p_Title);
		bool InternalWriteLog(char* p_Function, int32_t p_line, char* p_Format, ...);
	};
}