#pragma once
#include <include/cef_v8.h>

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

					// TODO: Actually get AnvilOnline build
					std::string s_Version("AnvilOnline Client Version: ");
					s_Version += "Pre-Alpha";

					p_RetVal = CefV8Value::CreateString(s_Version.c_str());
					return true;
				}

			protected:
				IMPLEMENT_REFCOUNTING(GetVersionHandler);
			};
		}
	}
}