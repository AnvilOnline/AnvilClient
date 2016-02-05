#include "network_session.hpp"

using namespace Network;

// network_session functions
network_session::update_user_properties_t network_session::update_user_properties = nullptr;
network_session::peer_request_player_desired_properties_update_t network_session::peer_request_player_desired_properties_update = nullptr;
network_session::join_remote_session_t network_session::join_remote_session = nullptr;
network_session::create_session_for_remote_join_t network_session::create_session_for_remote_join = nullptr;