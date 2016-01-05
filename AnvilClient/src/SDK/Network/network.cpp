#include "Network.hpp"

using namespace Network;

// Network Functions
network::get_player_index_t network::get_player_index = nullptr;
network::xnaddrtoinaddr_t network::xnaddrtoinaddr = nullptr;
network::transport_address_valid_t network::transport_address_valid = nullptr;
network::network_life_cycle_create_local_squad_t network::network_life_cycle_create_local_squad = nullptr;
network::session_interface_update_local_state_t network::session_interface_update_local_state = nullptr;
network::session_interface_update_session_t network::session_interface_update_session = nullptr;
