#pragma once
#include <include/cef_v8.h>
#include <include/cef_frame.h>
#include <include/wrapper/cef_message_router.h>
namespace Anvil
{
	namespace Client
	{
		namespace Rendering
		{
			namespace Handlers
			{

				class MainHandler : public CefMessageRouterBrowserSide::Handler
				{
				public:
					MainHandler();

					virtual bool OnQuery(CefRefPtr<CefBrowser> p_Browser, CefRefPtr<CefFrame> p_Frame, int64 p_QueryID, const CefString& p_Request, bool p_Persistent, CefRefPtr<Callback> p_Callback) override;

				private:
					static std::string GetFrameName(CefRefPtr<CefFrame> p_Frame);
					static std::string EscapeString(const std::string& p_String);

					CefRefPtr<CefFrame> m_Frame;
				};
			}
		}
	}
}