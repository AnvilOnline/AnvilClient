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
				typedef std::function<bool(const CefV8ValueList&)> BridgeFunc;

				class WebRendererHandler : public CefV8Handler
				{
					std::string m_Name;
					CefRefPtr<CefV8Value> m_Object;

					std::vector < std::pair<std::string, CefRefPtr<CefV8Value>>> m_CefFunctions;
					std::vector<std::pair < std::string, BridgeFunc>> m_Functions;

				public:
					virtual bool Execute(const CefString& p_Name, CefRefPtr<CefV8Value> p_Object, const CefV8ValueList& p_Arguments, CefRefPtr<CefV8Value>& p_RetVal, CefString& p_Exception) override
					{
						for (auto& l_Pair : m_Functions)
						{
							auto l_Name = l_Pair.first;
							auto l_Function = l_Pair.second;
							if (!l_Function)
								continue;

							if (p_Name == l_Name)
								return l_Function(p_Arguments);
						}
						return false;
					}

					WebRendererHandler(std::string p_HandlerName)
						: m_Name(p_HandlerName)
					{
						m_Object = CefV8Value::CreateObject(nullptr);
					}
					bool AddFunction(std::string p_FunctionName, BridgeFunc p_Function)
					{
						for (auto& l_Pair : m_Functions)
							if (l_Pair.first == p_FunctionName)
								return false;

						
					}
				protected:
					IMPLEMENT_REFCOUNTING(WebRendererHandler);
				};
			}
		}
	}
}