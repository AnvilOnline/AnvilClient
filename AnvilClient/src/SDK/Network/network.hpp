#pragma once
#include <stdint.h>

namespace Network
{
	class network
	{
	public:

		// Functions

		// TODO: Parameters
		typedef int32_t(__fastcall* get_player_index_t)(void* v0, void* v1);
		static get_player_index_t get_player_index;

		// TODO: Verify the prototype is correct
		typedef bool(__cdecl* xnaddrtoinaddr_t)(void* v0);
		static xnaddrtoinaddr_t xnaddrtoinaddr;

		typedef bool(__thiscall* transport_address_valid_t)(void* This);
		static transport_address_valid_t transport_address_valid;

		typedef bool(__thiscall* network_life_cycle_create_local_squad_t)(void* This);
		static network_life_cycle_create_local_squad_t network_life_cycle_create_local_squad;

		typedef void*(__cdecl* session_interface_update_local_state_t)();
		static session_interface_update_local_state_t session_interface_update_local_state;

		typedef uint32_t(__fastcall* session_interface_update_session_t)(void* v0);
		static session_interface_update_session_t session_interface_update_session;
	};
}