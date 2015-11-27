#pragma once
#include <include/cef_v8.h>
#include <Client/AnvilClient.hpp>

namespace Anvil
{
	namespace Client
	{
		namespace Rendering
		{
			class GetVersionHandler : public CefV8Handler
			{
			public:
				GetVersionHandler()
				{

				}

				virtual bool Execute(const CefString& p_Name, CefRefPtr<CefV8Value> p_Object, const CefV8ValueList& p_Arguments, CefRefPtr<CefV8Value>& p_RetVal, CefString& p_Exception) override
				{
					if (p_Name != "GetVersion")
						return false;

					p_RetVal = CefV8Value::CreateString(AnvilClient::GetInstance()->GetVersion());
					return true;
				}

			protected:
				IMPLEMENT_REFCOUNTING(GetVersionHandler);
			};
		}
	}
}