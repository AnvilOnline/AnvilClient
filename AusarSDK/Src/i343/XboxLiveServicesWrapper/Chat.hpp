#pragma once
#include <i343/XboxLiveServicesWrapper.hpp>

namespace i343
{
	class XboxLiveServicesWrapper::Chat
	{
	public:
		struct ChatPerformanceCounters;
		struct ChatUser;
		enum MessageType;
		enum TraceLevel;
		enum Quality;
	};
}