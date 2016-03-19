#pragma once
#include <include/cef_v8.h>
#include <Utils/Logger.hpp>
#include <Client/Rendering/WebRenderer.hpp>
#include <Client/AnvilClient.hpp>
#include <Utils/Util.hpp>
#include <SDK/Unsorted/simulation_sandbox_engine_globals_definition.h>

namespace Anvil
{
	namespace Client
	{
		namespace Rendering
		{
			namespace Bridge
			{
				class ClientFunctions
				{
				public:

					static bool OnGetVersion(const CefV8ValueList& p_Arguments, CefRefPtr<CefV8Value>& p_RetVal)
					{
						p_RetVal = CefV8Value::CreateString(AnvilClient::GetInstance()->GetVersion());

						return true;
					}

					static bool OnConnect(const CefV8ValueList& p_Arguments, CefRefPtr<CefV8Value>& p_RetVal)
					{
						WriteLog("Client connect called.");

						return true;
					}

					static bool OnLoadMap(const CefV8ValueList& p_Arguments, CefRefPtr<CefV8Value>& p_RetVal)
					{
						if (p_Arguments.size() < 3)
						{
							WriteLog("LoadMap not called with proper arguments.");
							p_RetVal = CefV8Value::CreateBool(false);
							return true;
						}

						auto s_MapName = p_Arguments[0]->GetStringValue().ToString();
						auto s_GameMode = p_Arguments[1]->GetIntValue();
						auto s_GameType = p_Arguments[2]->GetIntValue();

						AnvilClient::GetInstance()->ForceLoadMap(s_MapName, s_GameMode, s_GameType);

						p_RetVal = CefV8Value::CreateBool(true);
						Sleep(25);
						
						auto s_WebRenderer = Rendering::WebRenderer::GetInstance();
						s_WebRenderer->ShowRenderer(!s_WebRenderer->IsRendering(), false);

						return true;
					}

					static bool OnQuit(const CefV8ValueList& p_Arguments, CefRefPtr<CefV8Value>& p_RetVal)
					{
						// TODO: Implement AnvilClient::Shutdown to ensure all resources get freed properly.

						p_RetVal = CefV8Value::CreateBool(true);

						AnvilClient::GetInstance()->Shutdown();

						return true;
					}
				};
			}
		}
	}
}
