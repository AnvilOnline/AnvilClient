#pragma once
#include <i343/XboxLiveServicesWrapper.hpp>
#include <i343/XboxLiveServicesWrapper/Chat.hpp>

#include <cstdint>
#include <functional>

namespace i343
{
	class XboxLiveServicesWrapper::XboxLiveServicesEngineInterface
	{
	public:
		typedef std::function<void(Chat::MessageType, uint8_t, uint8_t* const, uint32_t, uint64_t)> PacketReadyCallback;

		typedef void(*ChatManager_CreateSession_t)(XboxLiveServicesEngineInterface* p_This);
		static ChatManager_CreateSession_t ChatManager_CreateSession;

		typedef uint8_t(*ChatManager_GetMaxChannels_t)(XboxLiveServicesEngineInterface* p_This);
		static ChatManager_CreateSession_t ChatManager_CreateSession;

		typedef void(*ChatManager_GetPerformanceCounters_t)(XboxLiveServicesEngineInterface* p_This, Chat::ChatPerformanceCounters* p_Counters);
		static ChatManager_GetPerformanceCounters_t ChatManager_GetPerformanceCounters;

		typedef bool(*ChatManager_GetUser_t)(XboxLiveServicesEngineInterface* p_This, Chat::ChatUser* p_User);
		static ChatManager_GetUser_t ChatManager_GetUser;

		typedef bool(*ChatManager_HasMicFocus_t)(XboxLiveServicesEngineInterface* p_This);
		static ChatManager_HasMicFocus_t ChatManager_HasMicFocus;

		typedef bool(*ChatManager_IsSessionReady_t)(XboxLiveServicesEngineInterface* p_This);
		static ChatManager_IsSessionReady_t ChatManager_IsSessionReady;

		typedef void(*ChatManager_OnIncomingChatMessage_t)(XboxLiveServicesEngineInterface* p_This, const uint8_t*, uint32_t);
		static ChatManager_OnIncomingChatMessage_t ChatManager_OnIncomingChatMessage;

		typedef void(*ChatManager_OnNewRemoteConsole_t)(XboxLiveServicesEngineInterface* p_This, int32_t);
		static ChatManager_OnNewRemoteConsole_t ChatManager_OnNewRemoteConsole;

		typedef void(*ChatManager_RefreshUserCache_t)(XboxLiveServicesEngineInterface* p_This);
		static ChatManager_RefreshUserCache_t ChatManager_RefreshUserCache;

		typedef void(*ChatManager_RegisterPacketReadyCallback_t)(XboxLiveServicesEngineInterface* p_This, PacketReadyCallback p_Callback);
		static ChatManager_RegisterPacketReadyCallback_t ChatManager_RegisterPacketReadyCallback;

		typedef void(*ChatManager_SetDiagnostics_t)(XboxLiveServicesEngineInterface* p_This, Chat::TraceLevel p_Level, bool);
		static ChatManager_SetDiagnostics_t ChatManager_SetDiagnostics;

		typedef void(*ChatManager_SetEncodingQuality_t)(XboxLiveServicesEngineInterface* p_This, Chat::Quality p_Quality);
		static ChatManager_SetEncodingQuality_t ChatManager_SetEncodingQuality;

		typedef void(*ChatManager_SetJitterBufferProperties_t)(XboxLiveServicesEngineInterface* p_This, uint32_t, uint32_t, uint32_t);
		static ChatManager_SetJitterBufferProperties_t ChatManager_SetJitterBufferProperties;

		typedef void(*ChatManager_SetSuppressPacketProcessing_t)(XboxLiveServicesEngineInterface* p_This, bool p_Suppress);
		static ChatManager_SetSuppressPacketProcessing_t ChatManager_SetSuppressPacketProcessing;

	};
}