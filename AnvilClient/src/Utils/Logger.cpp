#include "Logger.hpp"
#include <Misc/BuildInfo.hpp>
#include <sstream>
#include <consoleapi.h>

using Anvil::Utils::Logger;

Logger* Logger::m_Instance = nullptr;
Logger* Logger::GetInstance()
{
	if (!m_Instance)
		m_Instance = new Logger;
	return m_Instance;
}

bool Logger::Init()
{
	std::stringstream s_Stream;
	s_Stream << "AnvilOnline Alpha Build: " << __DATE__ << "-" << ANVIL_BUILD;
	auto s_Version = s_Stream.str();
	
	try
	{
		// Try to open the log file if one exists and append to it
		m_Stream.open("anvil-runtime.log", std::ofstream::out | std::ofstream::app);
	}
	catch (std::exception& p_Exception)
	{
		WriteLog("Could not create log file %s.", p_Exception.what());
	}

	WriteLog("\r\n\r\nAnvil Init: %s", s_Version.c_str());

	InitConsole(s_Version);

	return true;
}

bool Logger::InitConsole(std::string p_Title)
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

bool Logger::InternalWriteLog(char* p_Function, int p_Line, char* p_Format, ...)
{
	va_list s_Args;
	va_start(s_Args, p_Format);

	auto s_FinalLength = _vscprintf(p_Format, s_Args) + 1;
	auto s_FinalString = static_cast<char*>(malloc(s_FinalLength));
	if (!s_FinalString)
		return false;

	vsprintf_s(s_FinalString, s_FinalLength, p_Format, s_Args);

	va_end(s_Args);

	std::stringstream s_Stream;
	s_Stream << "[" << p_Function << " : " << p_Line << "] " << s_FinalString << "\r\n";

	auto s_OutputString = s_Stream.str();

	// Output to the console first
	auto s_OutputLength = 0;
	if (m_ConsoleHandle && m_ConsoleHandle != INVALID_HANDLE_VALUE)
		WriteConsole(m_ConsoleHandle, s_OutputString.c_str(), s_OutputString.length(), reinterpret_cast<LPDWORD>(&s_OutputLength), nullptr);

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