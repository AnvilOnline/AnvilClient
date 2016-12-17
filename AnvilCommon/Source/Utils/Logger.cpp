#include "Logger.hpp"
#include <Globals.hpp>
#include <Utils/Util.hpp>

#include <sstream>
#include <consoleapi.h>
#include <vector>

using namespace AnvilCommon::Utils;

bool Logger::Init()
{
	auto s_Version = AnvilCommon::g_BuildInfo;

#if _DEBUG
	m_Debug = true;
#else
	m_Debug = Util::HasCommandLine("-debug");
#endif

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
	if (!m_Debug)
		return true;

	if (!AllocConsole())
		return false;

	// Set the console title
	SetConsoleTitle(p_Title.c_str());

	// Save our console handle for use later
	m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	return true;
}

bool Logger::InternalWriteLog(char* p_Function, int32_t p_Line, char* p_Format, ...)
{
	if (!m_Debug)
		return true;

	va_list s_Args;
	va_start(s_Args, p_Format);

	auto s_FinalLength = _vscprintf(p_Format, s_Args) + 1;

	std::vector<char> s_FinalString;
	s_FinalString.resize(s_FinalLength);
	fill(s_FinalString.begin(), s_FinalString.end(), 0);


	vsprintf_s(s_FinalString.data(), s_FinalLength, p_Format, s_Args);

	va_end(s_Args);

	std::stringstream s_Stream;
	s_Stream << "[" << p_Function << " : " << p_Line << "] " << s_FinalString.data() << "\r\n";

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