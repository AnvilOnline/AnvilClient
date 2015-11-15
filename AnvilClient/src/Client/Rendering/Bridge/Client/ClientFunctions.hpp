#pragma once
#include <include/cef_v8.h>
#include <Utils/Logger.hpp>

namespace Anvil
{
	namespace Client
	{
		namespace Rendering
		{
			class ClientFunctionsHandler : public CefV8Handler
			{
			public:
				ClientFunctionsHandler()
				{

				}

				virtual bool Execute(const CefString& p_Name, CefRefPtr<CefV8Value> p_Object, const CefV8ValueList& p_Arguments, CefRefPtr<CefV8Value>& p_RetVal, CefString& p_Exception) override
				{
					if (p_Name == "Quit")
					{
						WriteLog("Quit called.");
						Quit();
						return true;
					}

					if (p_Name == "Connect")
					{
						WriteLog("Client connect called.");
						return true;
					}

					return false;
				}

			protected:

				void Quit()
				{
					TerminateProcess(GetCurrentProcess(), 0);
				}

				IMPLEMENT_REFCOUNTING(ClientFunctionsHandler);
			};
		}
	}
}