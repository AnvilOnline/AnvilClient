#pragma once

namespace Anvil
{
	namespace Client
	{
		namespace Arch
		{
			enum ArchClientStatus
			{
				ACS_Disconnected,
				ACS_Disconnecting,
				ACS_Local,
				ACS_Connecting,
				ACS_Connected,

				// Extras, to be removed
				ACS_Banned,
				ACS_Temp
			};
		}
	}
}