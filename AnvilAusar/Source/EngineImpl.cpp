#include "EngineImpl.hpp"
#include <Windows.h>
#include <KnownFolders.h>
#include <ShlObj.h>

#include <Utils/Logger.hpp>

using namespace AnvilAusar;

bool EngineImpl::Init()
{
	if (!InitializeDirectories())
		return false;
	
	return true;
}

bool EngineImpl::InitializeDirectories()
{
	PWSTR s_LocalAppDataPath = nullptr;

	if (SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &s_LocalAppDataPath) != S_OK)
	{
		WriteLog("There was an error retrieving the local app data path.");
		return false;
	}

	std::wstring s_Path(s_LocalAppDataPath);

	m_LocalAppDataPath = std::string(s_Path.begin(), s_Path.end());
	m_LocalCachePath = m_LocalAppDataPath.substr(0, m_LocalAppDataPath.length() - 3) + "\\LocalCache";
	
	WriteLog("Local Application Data Path: %s.", m_LocalAppDataPath.c_str());

	return true;
}

uint8_t* EngineImpl::ExecutableBase()
{
	return nullptr;
}

size_t EngineImpl::ExecutableSize()
{
	return 0;
}

std::shared_ptr<AnvilCommon::IdCache> EngineImpl::GetStringCache()
{
	return nullptr;
}