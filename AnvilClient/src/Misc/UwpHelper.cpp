#include "UwpHelper.hpp"
#include <Windows.h>
#include <KnownFolders.h>
#include <Shlobj.h>
#include <Utils/Logger.hpp>

using namespace Anvil;

std::shared_ptr<UwpHelper> UwpHelper::GetInstance()
{
	static auto s_Instance = std::make_shared<UwpHelper>();
	return s_Instance;
}

bool UwpHelper::Init()
{
	return ReadDirectories();
}

std::wstring& UwpHelper::GetLocalUserPath()
{
	return m_UserLocalPath;
}

std::wstring & Anvil::UwpHelper::GetLocalCachePath()
{
	return m_LocalCachePath;
}

bool UwpHelper::ReadDirectories()
{
	PWSTR s_Path = nullptr;
	if (SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &s_Path) != S_OK)
		return false;

	m_UserLocalPath = s_Path;

	// The path returned is "C:\Users\USERNAME\AppData\Local\Packages\microsoft.halo5forge_8wekyb3d8bbwe\AC"
	m_LocalCachePath = m_UserLocalPath.substr(0, m_UserLocalPath.length() - 3) + L"\\LocalCache";

	WriteLog(L"LocalAppDataPath: %s.", m_UserLocalPath.c_str());

	return true;
}