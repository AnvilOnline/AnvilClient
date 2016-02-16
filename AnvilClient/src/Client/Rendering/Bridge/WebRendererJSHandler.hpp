#pragma once
#include <string>
#include <include/cef_v8.h>
#include <functional>

namespace Anvil
{
	namespace Client
	{
		namespace Rendering
		{
			namespace Bridge
			{
				typedef std::function<bool(const CefV8ValueList& p_Arguments, CefRefPtr<CefV8Value>& p_RetVal)> BridgeFunc;

				class WebRendererJSHandler : public CefV8Handler
				{
					std::string m_Name;
					CefRefPtr<CefV8Value> m_Object;

					std::vector < std::pair<std::string, CefRefPtr<CefV8Value>>> m_CefFunctions;
					std::vector<std::pair < std::string, BridgeFunc>> m_Functions;

				public:
					WebRendererJSHandler(std::string p_HandlerName);

					virtual bool Execute(const CefString& p_Name, CefRefPtr<CefV8Value> p_Object, const CefV8ValueList& p_Arguments, CefRefPtr<CefV8Value>& p_RetVal, CefString& p_Exception) override;
					bool AddFunction(std::string p_FunctionName, BridgeFunc p_Function);

					std::string GetName();
					CefRefPtr<CefV8Value> GetHandlerObject();

				protected:
					IMPLEMENT_REFCOUNTING(WebRendererJSHandler);
				};
			}
		}
	}
}