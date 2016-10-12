#pragma once
#include <i343/XboxLiveServicesWrapper.hpp>

namespace i343
{
	class XboxLiveServicesWrapper::UserFacade
	{
	public:
		/* TODO: Structure */

		// Static Function, figure out how to declare this properly
		//typedef UserFacade* (*Get_t)();

		typedef i343::XboxLiveServicesWrapper::UserFacade* (*newInstance_t)(i343::XboxLiveServicesWrapper::UserFacade* th);
		static newInstance_t newInstance;

		typedef void* (*destroyInstance_t)(i343::XboxLiveServicesWrapper::UserFacade* th);
		static destroyInstance_t destroyInstance;
	};
}