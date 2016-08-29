#pragma once
#include <functional>
#include "NetProtocol.hpp"

enum ActiveUserIndex;
enum eSaveGameType;

namespace i343
{
	class XboxLiveServicesWrapper
	{
	public:
		enum PartyJoinability;

		// Classes
		// Should this be a namespace???
		class UserFacade
		{
		public:
			UserFacade& Get();
		};

		class License;
		class Chat
		{
		public:
			enum eAddRemoveAction;
			struct ChatUser;
			struct ChatPerformanceCounters;
		};

		class Privacy
		{
		public:
			enum eResult;
			enum ePermissionId;
		};

		class Session
		{
		public:
			struct SessionDoc;
		};

		// Enumerations
		enum XboxOnePrivilege;
		enum XboxOnePrivilegeCheckResult;

		class details
		{
			class XboxLiveServicesEngineInterfaceInternal
			{
			public:
				bool LobbyPartyManager_BuildPartyRosterUpdateRequest(class std::function<void(struct i343::NetProtocol::LobbyService::PartyRosterUpdateRequest const *)>);
				bool LobbyPartyManager_CanLeaveParty(void);
				bool LobbyPartyManager_HasActiveParty(void);
				bool LobbyPartyManager_IsLocalClientPartyArbiter(void);

				bool TMS_UserCompletionEventTriggered(void);
				bool UserManager_HasSignedInUser(void);
				// TODO: Figure out if there is any difference between i343::STLVector and std::vector
				// bool LobbyPartyManager_GetCurrentPartyMembers(void);

				PartyJoinability LobbyPartyManager_GetJoinability(void);
				_GUID LobbyPartyManager_GetPartyGuid(void);
				uint64_t LobbyPartyManager_GetActiveLocalPartyMemberCount(void);
				uint64_t LobbyPartyManager_GetActivePartyMemberCount(void);
				uint64_t UserManager_GetCurrentXuid(void);
				uint64_t UserManager_GetGamepadIdFromXuid(unsigned __int64);

				void ChatManager_AddRemoveUser(class std::tuple<enum  i343::XboxLiveServicesWrapper::Chat::eAddRemoveAction, unsigned __int64, unsigned char> const &, class std::function<void(void)>);
				void ChatManager_RemoveRemoteConsole(class std::tuple<long> const &, class std::function<void(void)>);
				void CloudCompute_CheckClusterAllocation(class std::tuple<char const *, char const *, char const *> const &, class std::function<void(bool)>);
				// void ConnectedStorage_GetFileInfo(class std::tuple<enum  ActiveUserIndex, wchar_t const *> const &, class std::function<void(class i343::STLVector<struct i343::XboxLiveServicesWrapper::SaveGameMetadata, class i343::STLAllocParams<1, 5>, class i343::STLAllocator<struct i343::XboxLiveServicesWrapper::SaveGameMetadata, class i343::STLAllocParams<1, 5>>>)>);
				void ConnectedStorage_GetItemCount(class std::tuple<enum  ActiveUserIndex> const &, class std::function<void(unsigned int)>);
				// void ConnectedStorage_GetItemInfo(class std::tuple<enum  ActiveUserIndex> const &, class std::function<void(class i343::STLVector<struct i343::XboxLiveServicesWrapper::SaveGameMetadata, class i343::STLAllocParams<1, 5>, class i343::STLAllocator<struct i343::XboxLiveServicesWrapper::SaveGameMetadata, class i343::STLAllocParams<1, 5>>>)>);
				// void ConnectedStorage_GetItemMetadata(class std::tuple<enum  ActiveUserIndex> const &, class std::function<void (class i343::STLVector<struct i343::XboxLiveServicesWrapper::SaveGameMetadata, class i343::STLAllocParams<1, 5>, class i343::STLAllocator<struct i343::XboxLiveServicesWrapper::SaveGameMetadata, class i343::STLAllocParams<1, 5>>>)>);
				// void GameSession_GetCorrelationId(class std::tuple<char const *, char const *, char const *> const &, class std::function<void (class i343::STLStringWithAllocator<wchar_t, class i343::STLAllocParams<1, 51>, class i343::STLAllocator<wchar_t, class i343::STLAllocParams<1, 51>>>)>);
				void GameSession_GetSessionDocument(class std::tuple<char const *, char const *, char const *> const &, class std::function<void(struct i343::XboxLiveServicesWrapper::Session::SessionDoc)>);
				void GameSession_JoinGameSessionDocument(class std::tuple<char const *, char const *, char const *> const &, class std::function<void(bool)>);
				void GameSession_LeaveGameSessionDocument(class std::tuple<char const *, char const *, char const *> const &, class std::function<void(bool)>);
				// void LobbyPartyManager_CreatePartyOfLocalUsers(class std::tuple<class i343::STLVector<unsigned long, class i343::STLAllocParams<1, 5>, class i343::STLAllocator<unsigned long, class i343::STLAllocParams<1, 5>>>> const &, class std::function<void (long)>);
				// void LobbyPartyManager_GetPartyInfoBatch(class std::tuple<enum  ActiveUserIndex, class i343::STLVector<unsigned __int64, class i343::STLAllocParams<1, 5>, class i343::STLAllocator<unsigned __int64, class i343::STLAllocParams<1, 5>>>, bool> const &, class std::function<void(class i343::STLVector<struct i343::XboxLiveServicesWrapper::SocialAndPresence::XuidPartyInfo, class i343::STLAllocParams<1, 5>, class i343::STLAllocator<struct i343::XboxLiveServicesWrapper::SocialAndPresence::XuidPartyInfo, class i343::STLAllocParams<1, 5>>>)>);
				// void LobbyPartyManager_InviteToGame(class std::tuple<class i343::STLVector<unsigned __int64, class i343::STLAllocParams<1, 5>, class i343::STLAllocator<unsigned __int64, class i343::STLAllocParams<1, 5>>>, unsigned int, wchar_t const *, wchar_t const *> const &, class std::function<void (bool)>);
				void LobbyPartyManager_RegisterLeaveLobbyCallback(class std::function<void(void)> const &);
				void LobbyPartyManager_RegisterRosterChangedCallback(class std::function<void(void)> const &);
				void LobbyPartyManager_SetJoinability(enum  i343::XboxLiveServicesWrapper::PartyJoinability);
				void LobbyPartyManager_SetPartyGuid(struct _GUID);
				void LobbyPartyManager_UpdateMPSDInfo(class std::tuple<char const *, char const *, char const *> const &, class std::function<void(bool)>);

				// void XboxLiveServicesEngineInterfaceInternal::MarketPlace_GetCatalogInfo(class std::tuple<unsigned long, wchar_t const *> const &, class std::function<void(struct i343::XboxLiveServicesWrapper::MarketPlace::ProductInfo)>);
				// void MarketPlace_GetSignedOffers(class std::tuple<unsigned long, class i343::STLVector<class s_static_wstring<37>, class i343::STLAllocParams<1, 5>, class i343::STLAllocator<class s_static_wstring<37>, class i343::STLAllocParams<1, 5>>> const &> const &, class std::function<void(class i343::STLVector<struct i343::XboxLiveServicesWrapper::MarketPlace::OfferInfo, class i343::STLAllocParams<1, 5>, class i343::STLAllocator<struct i343::XboxLiveServicesWrapper::MarketPlace::OfferInfo, class i343::STLAllocParams<1, 5>>>)>);
				void MarketPlace_ShowPurchaseUIAsync(class std::tuple<unsigned long, wchar_t const *> const &, class std::function<void(bool)>);
				// void ProfileManager_GetProfile(class std::tuple<enum  ActiveUserIndex, unsigned __int64> const &, class std::function<void(struct i343::XboxLiveServicesWrapper::ProfileService::Profile)>);
				// void ProfileManager_GetProfiles(class std::tuple<enum  ActiveUserIndex, class i343::STLVector<unsigned __int64, class i343::STLAllocParams<1, 5>, class i343::STLAllocator<unsigned __int64, class i343::STLAllocParams<1, 5>>>> const &, class std::function<void (class i343::STLVector<struct i343::XboxLiveServicesWrapper::ProfileService::Profile, class i343::STLAllocParams<1, 5>, class i343::STLAllocator<struct i343::XboxLiveServicesWrapper::ProfileService::Profile, class i343::STLAllocParams<1, 5>>>)>);
				// void SocialManager_GetSocialRelationships(class std::tuple<enum  ActiveUserIndex, unsigned long> const &, class std::function<void (class i343::STLMap<unsigned __int64, struct i343::XboxLiveServicesWrapper::SocialAndPresence::XboxSocialRelationship, class i343::STLAllocParams<1, 5>, struct std::less<unsigned __int64>, class i343::STLAllocator<struct std::pair<unsigned __int64 const, struct i343::XboxLiveServicesWrapper::SocialAndPresence::XboxSocialRelationship>, class i343::STLAllocParams<1, 5>>>)>);
				// void TMS_GetServiceConfig(class std::tuple<struct i343::XboxLiveServicesWrapper::TMS_ServiceConfig *> const &, class std::function<void (bool)>);
				// void TMS_GetTitleStorageSaveGameMetadata(class std::tuple<bool> const &, class std::function<void (class i343::STLVector<struct i343::XboxLiveServicesWrapper::SaveGameMetadata, class i343::STLAllocParams<1, 5>, class i343::STLAllocator<struct i343::XboxLiveServicesWrapper::SaveGameMetadata, class i343::STLAllocParams<1, 5>>>)>);
				
				// void UserManager_CheckPrivilege(class std::tuple<unsigned long, enum  i343::XboxLiveServicesWrapper::XboxOnePrivilege, bool> const &, class std::function<void (enum  i343::XboxLiveServicesWrapper::XboxOnePrivilegeCheckResult)>);
				// void UserManager_GetTokenForAllUsers(class std::tuple<wchar_t const *, class std::function<void (struct i343::XboxLiveServicesWrapper::XblTokenResult const *)>> const &, class std::function<void (long)>);
				void UserManager_PickUserForGamepad(class std::tuple<unsigned __int64, bool> const &, class std::function<void(unsigned long)>);
				void UserManager_ShowAddRemoveFriend(class std::tuple<enum  ActiveUserIndex, unsigned __int64> const &, class std::function<void(void)>);
				void UserManager_ShowProfileCard(class std::tuple<enum  ActiveUserIndex, unsigned __int64> const &, class std::function<void(void)>);
				void UserManager_ShowSendInvites(class std::tuple<enum  ActiveUserIndex, char const *, char const *, char const *> const &, class std::function<void(void)>);
			};
		};

		class XboxLiveServicesEngineInterface
		{
		public:
			void OnResourceFull(void);
			void OnResume(void);
			void ChatManager_GetMaxChannels(void);
			void GetThunderheadUdpGamePort(void);
			void ChatManager_CreateSession(void);
			//void ChatManager_GetPerformanceCounters(i343::XboxLiveServicesWrapper::Chat::ChatPerformanceCounters *);
			void ChatManager_OnIncomingChatMessage(long, unsigned char const *, unsigned int);
			void ChatManager_OnNewRemoteConsole(long);
			void ChatManager_RefreshUserCache(void);
			// void ChatManager_RegisterPacketReadyCallback(class std::function<void (enum  i343::XboxLiveServicesWrapper::Chat::MessageType, unsigned char, long *, unsigned char const *, unsigned int, unsigned __int64)> const &);
			void ChatManager_SetJitterBufferProperties(unsigned long, unsigned long, unsigned long);
			void ChatManager_SetSuppressPacketProcessing(bool);
			void ChatManager_TeardownSession(void);
			void ClearThunderheadHostname(void);
			void ConnectedStorageManager_DeleteContainerByName(enum  ActiveUserIndex, wchar_t const *);
			void ConnectedStorageManager_LoadBuffer(enum  ActiveUserIndex, wchar_t const *, long, class std::function<void(long, void *, unsigned __int64)> const &);
			void ConnectedStorageManager_LoadBuffer(unsigned __int64, wchar_t const *, long, class std::function<void(long, void *, unsigned __int64)> const &);
			void ConnectedStorageManager_SaveBuffer(enum  ActiveUserIndex, wchar_t const *, wchar_t const *, unsigned char *, long, enum  eSaveGameType, class std::function<void(bool, enum  eSaveGameType)> const &);
			void ConnectedStorageManager_SaveBuffer(unsigned __int64, wchar_t const *, wchar_t const *, unsigned char *, long, enum  eSaveGameType, class std::function<void(bool, enum  eSaveGameType)> const &);
			void CreateSecurityAssociation(wchar_t *);
			void Dispose(void);
			// void Initialize(struct i343::XboxLiveServicesWrapper::XboxLiveServicesEngineInterface::servicesEngineInitialization const &);
			// void ParseQoSServerEndpointsFromPayload(char const *, unsigned __int64, class i343::STLVector<class std::shared_ptr<struct i343::XboxLiveServicesWrapper::ThunderheadDataCenter>, class i343::STLAllocParams<1, 5>, class i343::STLAllocator<class std::shared_ptr<struct i343::XboxLiveServicesWrapper::ThunderheadDataCenter>, class i343::STLAllocParams<1, 5>>> &);
			void PartyChat_SetSuppressed(bool);
			void PrivacyCache_Flush(void);
			void SetRichPresence(unsigned __int64, wchar_t const *, bool, wchar_t const *);
			void TitleStorageManager_DeleteContainerByName(wchar_t const *);
			void TitleStorageManager_LoadBuffer(wchar_t const *, long, class std::function<void(long, void *, unsigned __int64)> const &);
			void TitleStorageManager_SaveBuffer(wchar_t const *, wchar_t const *, unsigned char *, long);
			void UseRawUDPInsteadOfIPsec(void);
		};

		void ConnectedStorageManager_SaveBuffer(ActiveUserIndex, wchar_t const *, wchar_t const *, unsigned char *, long, eSaveGameType, std::function<void(bool, eSaveGameType)> const &);
		void ConnectedStorageManager_LoadBuffer(ActiveUserIndex, wchar_t const *, long, std::function<void(long, void *, unsigned __int64)> const &);
		void ConnectedStorageManager_DeleteContainerByName(ActiveUserIndex, wchar_t const *);

		void TitleStorageManager_SaveBuffer(wchar_t const *, wchar_t const *, unsigned char *, long);
		void TitleStorageManager_LoadBuffer(wchar_t const *, long, class std::function<void(long, void *, unsigned __int64)> const &);
		void TitleStorageManager_DeleteContainerByName(wchar_t const *);
		
		XboxLiveServicesEngineInterface* Get();
	};
}