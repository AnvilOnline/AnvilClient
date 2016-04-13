#pragma once
#include <string>
#include <objbase.h>
#include "ArchStatus.hpp"

namespace Anvil
{
	namespace Client
	{
		namespace Arch
		{
			class ArchClient
			{
				// TODO: Implement automatic failover load balancing
				std::wstring m_HostNode;

				// User information
				std::wstring m_Username;
				GUID m_Identity;
				GUID m_NodeToken;

				// Current status
				ArchClientStatus m_Status;

				
			public:
				ArchClient(std::wstring p_Host);

				bool Login(std::wstring p_Username, std::wstring p_Password);
			};
		}
	}
}