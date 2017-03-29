#pragma once
#include <WinSock2.h>
#include <Interfaces\Initializable.hpp>

#include <Blam\Network\Session.hpp>

namespace AnvilEldorado
{
	class Network : 
		public AnvilCommon::Initializable
	{
	public:
		bool Init() override;

		bool StartServer();
		bool StopServer();

		std::string GetServerName() const;
		void SetServerName(const std::string &p_ServerName);

		std::string GetServerMessage() const;
		void SetServerMessage(const std::string &p_ServerMessage);

		std::string GetServerPassword() const;
		void SetServerPassword(const std::string &p_ServerPassword);

		uint32_t GetServerCountdown() const;
		void SetServerCountdown(const uint32_t p_ServerCountdown);

		uint32_t GetServerMaximumPlayers() const;
		void SetServerMaximumPlayers(const uint32_t p_ServerMaximumPlayers);

		uint32_t GetServerPort() const;
		void SetServerPort(const uint32_t p_Port);

		uint32_t GetServerGamePort() const;
		void SetServerGamePort(const uint32_t p_ServerGamePort);

		void OnPongReceived(const Blam::Network::NetworkAddress &p_From, const Blam::Network::PongPacket &p_Pong, uint32_t p_Latency);
		void OnLifeCycleStateChanged(const Blam::Network::LifeCycleState &p_NewState);

	private:
		SOCKET m_ServerSocket;
		bool m_ServerSocketOpen = false;

		std::string m_ServerName = "Halo: Online";
		std::string m_ServerMessage = "";
		std::string m_ServerPassword = "";
		uint32_t m_ServerCountdown = 5;
		uint32_t m_ServerMaximumPlayers = 16;
		uint32_t m_ServerPort = 11775;
		uint32_t m_ServerGamePort = 0;
		bool m_ServerShouldAnnounce;
	};
}