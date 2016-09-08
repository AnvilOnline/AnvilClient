#pragma once
#include <Misc/IInit.hpp>
#include <Misc/Macros.hpp>

namespace Anvil
{
	namespace Client
	{
		namespace Hooks
		{
			class WinHooks : public IInit
			{
			private:
				DeclareFunction(HANDLE, CreateFileW, WINAPI, LPCWSTR lpFilename, DWORD dwAccess, DWORD dwSharing, LPSECURITY_ATTRIBUTES saAttributes, DWORD dwCreation, DWORD dwAttributes, HANDLE hTemplate);

				void Hook_FileIo();

			public:
				WinHooks();
				bool Init() override;
			};
		}
	}
}