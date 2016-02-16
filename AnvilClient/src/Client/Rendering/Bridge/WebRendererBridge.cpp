#include "WebRendererBridge.hpp"
#include "WebRendererJSHandler.hpp"

using namespace Anvil::Client::Rendering::Bridge;

std::shared_ptr<WebRendererJSHandler> WebRendererBridge::CreateHandler(std::string p_HandlerName)
{
	return std::make_shared<WebRendererJSHandler>(p_HandlerName);
}

bool WebRendererBridge::AddHandler(std::shared_ptr<WebRendererJSHandler> p_Handler)
{
	if (!p_Handler)
		return false;

	auto s_HandlerName = p_Handler->GetName();

	for (auto& l_Handler : m_Handlers)
	{
		if (!l_Handler)
			continue;

		if (l_Handler->GetName() == s_HandlerName)
			return false;
	}

	auto s_Global = CefV8Context::GetCurrentContext()->GetGlobal();
	if (!s_Global)
		return false;

	if (!s_Global->SetValue(CefString(s_HandlerName), p_Handler->GetHandlerObject(), V8_PROPERTY_ATTRIBUTE_NONE))
		return false;

	m_Handlers.push_back(p_Handler);

	return true;
}
