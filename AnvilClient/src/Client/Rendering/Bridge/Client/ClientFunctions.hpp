#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <include/cef_v8.h>
#include <Utils/Logger.hpp>
#include <Client/Rendering/WebRenderer.hpp>
#include <Client/AnvilClient.hpp>
#include <Utils/Util.hpp>
#include <SDK/Unsorted/simulation_sandbox_engine_globals_definition.h>
#include <Client/Settings/SettingsManager.hpp>
#include <Client/Settings/SettingsGroup.hpp>

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

#include <string>
#include <sstream>
#include <memory>
#include <Client/Arch/ArchClient.hpp>
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

//using namespace web::http::experimental::listener;          // HTTP server
//using namespace web::experimental::web_sockets::client;     // WebSockets client
using namespace web::json;                                  // JSON library

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
						p_RetVal = CefV8Value::CreateBool(true);

						AnvilClient::GetInstance()->Shutdown();

						return true;
					}

					static bool OnGetSetting(const CefV8ValueList& p_Arguments, CefRefPtr<CefV8Value>& p_RetVal)
					{
						p_RetVal = CefV8Value::CreateBool(false);

						if (p_Arguments.empty())
							return false;

						// The setting should be in the format <settingGroup>.<settingName>
						auto s_SettingsString = p_Arguments[0]->GetStringValue().ToString();

						auto s_Index = s_SettingsString.find_first_of('.');
						if (s_Index == std::string::npos)
							return false;

						auto s_GroupString = s_SettingsString.substr(0, s_Index);
						auto s_SettingString = s_SettingsString.substr(s_Index + 1);

						auto s_SettingsManager = reinterpret_cast<Settings::SettingsManager*>(AnvilClient::GetInstance()->GetSettingsManager().get());
						if (!s_SettingsManager)
							return false;

						auto s_Group = s_SettingsManager->GetGroup(s_GroupString);
						return true;
					}

					static bool OnLogin(const CefV8ValueList& p_Arguments, CefRefPtr<CefV8Value>& p_RetVal)
					{
						p_RetVal = CefV8Value::CreateBool(false);

						if (p_Arguments.empty() || p_Arguments.size() != 2)
							return false;

						auto s_Username = p_Arguments[0]->GetStringValue().ToWString();
						auto s_Password = p_Arguments[1]->GetStringValue().ToWString();

						auto s_Client = std::make_shared<Client::Arch::ArchClient>(L"http://localhost:33642");
						
						p_RetVal = CefV8Value::CreateBool(s_Client->Login(s_Username, s_Password));

						return true;
					}
				};
			}
		}
	}
}
