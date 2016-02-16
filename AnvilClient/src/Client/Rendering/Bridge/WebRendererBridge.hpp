#pragma once
#include <string>
#include <include/cef_v8.h>
#include "WebRendererJSHandler.hpp"

namespace Anvil
{
	namespace Client
	{
		namespace Rendering
		{
			namespace Bridge
			{
				class WebRendererBridge
				{
					std::vector<std::shared_ptr<WebRendererJSHandler>> m_Handlers;

				public:
					std::shared_ptr<WebRendererJSHandler> CreateHandler(std::string p_HandlerName);

					bool AddHandler(std::shared_ptr<WebRendererJSHandler> p_Handler);
				};
			}
		}
	}
}