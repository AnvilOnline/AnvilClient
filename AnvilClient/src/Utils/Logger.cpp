#include "Logger.hpp"
#include <Misc/BuildInfo.hpp>
#include <sstream>
#include <consoleapi.h>
#include <vector>

#include <Misc/UwpHelper.hpp>

using Anvil::Utils::Logger;

std::shared_ptr<Logger> Logger::GetInstance()
{
	static auto s_Instance = std::make_shared<Logger>();
	return s_Instance;
}

bool Logger::Init()
{
	std::wstringstream s_Stream;
	s_Stream << L"AnvilOnline (H5F) Alpha Build: " << __DATE__ << L" - " << ANVIL_BUILD;
	auto s_Version = s_Stream.str();
	
	try
	{
		// Try to open the log file if one exists and append to it
		auto s_Path = UwpHelper::GetInstance()->GetLocalCachePath() + L"\\anvil-runtime.log";

		m_Stream.open(s_Path, std::wofstream::out | std::wofstream::app);
	}
	catch (std::exception& p_Exception)
	{
		WriteLog(L"Could not create log file %s.", p_Exception.what());
	}

	WriteLog(L"\r\n\r\nAnvil Init: %s.", s_Version.c_str());

	InitConsole(s_Version);

	return true;
}

bool Logger::InitConsole(std::wstring p_Title)
{
#ifndef _DEBUG
	// In release mode we don't log to console
	return true;
#else
	if (!AllocConsole())
		return false;

	// Set the console title
	SetConsoleTitle(p_Title.c_str());

	// Save our console handle for use later
	m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	return true;
#endif
}

bool Logger::InternalWriteLog(char* p_Function, int32_t p_Line, wchar_t* p_Format, ...)
{
	va_list s_Args;
	va_start(s_Args, p_Format);

	auto s_FinalLength = _vscwprintf(p_Format, s_Args) + 1;

	std::vector<wchar_t> s_FinalString;
	s_FinalString.resize(s_FinalLength);
	fill(s_FinalString.begin(), s_FinalString.end(), 0);


	vswprintf_s(s_FinalString.data(), s_FinalLength, p_Format, s_Args);

	va_end(s_Args);

	std::wstringstream s_Stream;
	s_Stream << L"[" << p_Function << L" : " << p_Line << L"] " << s_FinalString.data() << L"\r\n";

	auto s_OutputString = s_Stream.str();

	// Output to the console first
	DWORD s_OutputLength = 0;
	if (m_ConsoleHandle && m_ConsoleHandle != INVALID_HANDLE_VALUE)
		WriteConsole(m_ConsoleHandle, s_OutputString.c_str(), static_cast<DWORD>(s_OutputString.length()), &s_OutputLength, nullptr);

	// Try to log to file
	if (!m_Stream.is_open())
		return false;

	// Lock
	m_Lock.lock();

	// Write to our file stream
	m_Stream << s_OutputString.c_str();
	m_Stream.flush();

	// Release the lock
	m_Lock.unlock();

	return true;
}