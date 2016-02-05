#pragma once

namespace Network
{
	class network_session_parameter_remote_join_data
	{
	public:

		typedef bool(__thiscall* set_t)(network_session_parameter_remote_join_data* Th, const void* a2);
		static set_t set;
	};
}