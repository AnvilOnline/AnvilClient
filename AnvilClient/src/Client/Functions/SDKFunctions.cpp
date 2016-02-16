#include "SDKFunctions.h"
#include <SDK/Network/network_session.hpp>
#include <Utils/Util.hpp>
#include <Misc/Macros.hpp>
#include <SDK/Network/network_observer.hpp>

using namespace Anvil::Client::Functions;

bool SDKFunctions::Init()
{
	uint32_t s_BaseAddress = 0;
	uint32_t s_BaseSize = 0;

	if (!Utils::Util::GetExecutableInfo(s_BaseAddress, s_BaseSize))
		return false;


	// TODO: Figure out something to do with this crap

	// These functions are from 12.1.700255 and may not be accurate for other versions

	// TODO: Versioning

	// network_session::join_remote_session
	auto s_Network_network_session_join_remote_session = Utils::Util::FindPattern(reinterpret_cast<void*>(s_BaseAddress), s_BaseSize,
		"\x55\x8B\xEC\x81\xEC\x00\x00\x00\x00\x53\x56\x57\x68\x00\x00\x00\x00\x8D\x85\x00\x00\x00\x00\x6A\x00\x50\x89\x4D\xF4",
		"xxxxx????xxxx????xx????xxxxxx");
	if (s_Network_network_session_join_remote_session)
	{
		Network::network_session::join_remote_session = reinterpret_cast<Network::network_session::join_remote_session_t>(s_Network_network_session_join_remote_session);
		WriteLog("network_session::join_remote_session function found.");
	}
	else
		WriteLog("network_session::join_remote_session function NOT FOUND!");

	// network_session::create_session_for_remote_join
	auto s_Network_network_session_create_session_for_remote_join = Utils::Util::FindPattern(reinterpret_cast<void*>(s_BaseAddress), s_BaseSize,
		"\x55\x8B\xEC\x51\x53\x56\x57\x8B\x7D\x14",
		"xxxxxxxxxx");
	if (s_Network_network_session_create_session_for_remote_join)
	{
		Network::network_session::create_session_for_remote_join = reinterpret_cast<Network::network_session::create_session_for_remote_join_t>(s_Network_network_session_create_session_for_remote_join);
		WriteLog("network_session::create_session_for_remote_join function found.");
	}
	else
		WriteLog("network_session::create_session_for_remote_join function NOT FOUND!");

	// network_observer::secure_connection
	auto s_Network__network_observer__build_secure_connection = Utils::Util::FindPattern(reinterpret_cast<void*>(s_BaseAddress), s_BaseSize,
		"\x55\x8B\xEC\x8B\x45\x08\x83\xEC\x44",
		"xxxxxxxxx");
	if (s_Network__network_observer__build_secure_connection)
	{
		Network::network_observer::build_secure_connection = reinterpret_cast<Network::network_observer::build_secure_connection_t>(s_Network__network_observer__build_secure_connection);
		WriteLog("network_observer::build_secure_connection function found.");
	}
	else
		WriteLog("network_observer::build_secure_connection function NOT FOUND!");

	return true;
}
