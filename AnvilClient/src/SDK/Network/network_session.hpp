#pragma once

namespace Network
{
	class network_session
	{
	public:

		typedef unsigned int(__thiscall* update_user_properties_t)(network_session* This);
		static update_user_properties_t update_user_properties;

		typedef bool(__thiscall* peer_request_player_desired_properties_update_t)(network_session* This, void* v1, void* v2, void* p_Properties, void* v4);
		static peer_request_player_desired_properties_update_t peer_request_player_desired_properties_update;
	};
}