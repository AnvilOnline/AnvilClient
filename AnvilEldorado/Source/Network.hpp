#pragma once

#include <WinSock2.h>

#include "Interfaces\IInitializable.hpp"

#include "Utils\Singleton.hpp"

#include "Blam\Network\Session.hpp"

namespace AnvilEldorado
{
	class Network final : public AnvilCommon::IInitializable, public AnvilCommon::Singleton<Network>
	{
	public:
		bool Init() override;

		uint32_t GetServerPort() const;
		void SetServerPort(const uint32_t p_Port);

		bool StartInfoServer();
		bool StopInfoServer();

		typedef std::function<void(const Blam::Network::NetworkAddress &, uint32_t, uint16_t, uint32_t)> PongCallback;
		void OnPongReceived(const Blam::Network::NetworkAddress &p_From, const Blam::Network::PongPacket &p_Pong, uint32_t p_Latency);

		typedef std::function<void(Blam::Network::LifeCycleState)> LifeCycleStateChangedCallback;
		void OnLifeCycleStateChanged(const Blam::Network::LifeCycleState &p_NewState);

	private:
		SOCKET m_InfoSocket;
		bool m_InfoSocketOpen = false;

		uint32_t m_ServerPort = 11775;

		std::vector<PongCallback> m_PongCallbacks;

		std::vector<LifeCycleStateChangedCallback> m_LifeCycleStateChangedCallbacks;
	};
}