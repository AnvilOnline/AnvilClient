#include "network_observer.hpp"

using namespace Network;

network_observer::channel_send_message_t network_observer::channel_send_message = nullptr;
network_observer::observer_channel_send_message_t network_observer::observer_channel_send_message = nullptr;
network_observer::build_secure_connection_t network_observer::build_secure_connection = nullptr;