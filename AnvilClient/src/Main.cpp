#include <Client/AnvilClient.hpp>
#include <Utils/Logger.hpp>
#include <Windows.h>

#include <string>
#include <vector>
#include <stack>

#include <Shlobj.h>  // need to include definitions of constants

bool ListFiles(std::wstring path, std::wstring mask, std::vector<std::wstring>& files) {
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATAW ffd;
	std::wstring spec;
	std::stack<std::wstring> directories;

	directories.push(path);
	files.clear();

	while (!directories.empty()) {
		path = directories.top();
		spec = path + L"\\" + mask;
		directories.pop();

		hFind = FindFirstFileW(spec.c_str(), &ffd);
		if (hFind == INVALID_HANDLE_VALUE) {
			return false;
		}

		do {
			if (wcscmp(ffd.cFileName, L".") != 0 &&
				wcscmp(ffd.cFileName, L"..") != 0) {
				if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					directories.push(path + L"\\" + ffd.cFileName);
				}
				else {
					files.push_back(path + L"\\" + ffd.cFileName);
				}
			}
		} while (FindNextFileW(hFind, &ffd) != 0);

		if (GetLastError() != ERROR_NO_MORE_FILES) {
			FindClose(hFind);
			return false;
		}

		FindClose(hFind);
		hFind = INVALID_HANDLE_VALUE;
	}

	return true;
}

#include <istream>
#include <iostream>
#include <fstream>
#include <iterator>

uint32_t __stdcall Initialize(LPVOID)
{
	// Init our logger
	Anvil::Utils::Logger::GetInstance()->Init();

//#ifdef _DEBUG
//	WriteLog("Command Line Arguments: %s.", GetCommandLine());
//#endif
//
//	// Wait for DirectX 12 to get loaded
//	while (!GetModuleHandle("d3d12.dll"))
//		Sleep(250);
//
//	// Initialize the Anvil Client.
//	Anvil::Client::AnvilClient::GetInstance()->Init();



	std::vector<std::wstring> files;

	if (ListFiles(L".\\", L"*", files)) {
		for (std::vector<std::wstring>::iterator it = files.begin();
			it != files.end();
			++it) 
		{

			WriteLog("Found File: %S.", it->c_str());
		}
	}



	// .....

	//MessageBox(NULL, "WE GOT THIS SHIT", "ANVILS INJECTED SON", MB_OK);

	return S_OK;
}

bool __stdcall DllMain(void* p_Module, uint32_t p_Reason, void* p_Reserved)
{
	if (p_Reason == DLL_PROCESS_ATTACH)
		CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Initialize), nullptr, 0, nullptr);

	return true;
}