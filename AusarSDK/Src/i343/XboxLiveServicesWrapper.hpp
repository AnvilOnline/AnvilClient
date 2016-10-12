#pragma once

namespace i343
{
	class XboxLiveServicesWrapper
	{
	public:
		class Chat;
		class License;
		class UserFacade;
		class XboxLiveServicesEngineInterface;
		class XboxLiveServicesEngineInterfaceInternal;

		/* TODO: Class Definition */

		typedef void(*ForceRecheckLiveConnectivity_t)(i343::XboxLiveServicesWrapper* th);
		static ForceRecheckLiveConnectivity_t ForceRecheckLiveConnectivity;

		typedef void(*ForceRecheckTMS_t)(i343::XboxLiveServicesWrapper* th);
		static ForceRecheckTMS_t ForceRecheckTMS;

		typedef void(*ForceRefreshPartyView_t)(i343::XboxLiveServicesWrapper* th);
		static ForceRefreshPartyView_t ForceRefreshPartyView;

		typedef void(*ValidateUserOnResume_t)(i343::XboxLiveServicesWrapper* th);
		static ValidateUserOnResume_t ValidateUserOnResume;

	};
}