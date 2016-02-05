#pragma once
#include <stdint.h>

namespace Network
{
	class network_session
	{
	public:

		typedef uint32_t(__thiscall* update_user_properties_t)(network_session* This);
		static update_user_properties_t update_user_properties;

		typedef bool(__thiscall* peer_request_player_desired_properties_update_t)(network_session* This, void* v1, void* v2, void* p_Properties, void* v4);
		static peer_request_player_desired_properties_update_t peer_request_player_desired_properties_update;

		typedef bool(__thiscall* join_remote_session_t)(network_session* This, char a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, char a14);
		static join_remote_session_t join_remote_session;

		typedef bool(__thiscall* create_session_for_remote_join_t)(network_session* This, char a2, int a3, int a4, int a5, int a6, int a7, char a8, int a9, int a10, int a11, int a12, int a13);
		static create_session_for_remote_join_t create_session_for_remote_join;
	};
}