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
						return false;
						//p_RetVal = CefV8Value::CreateBool(false);

						//if (p_Arguments.empty() || p_Arguments.size() != 2)
						//	return false;

						//auto s_Username = p_Arguments[0]->GetStringValue().ToString();
						//auto s_Password = p_Arguments[1]->GetStringValue().ToString();

						//// TODO: Hash Password

						//// TODO: Request login

						//// Create Request
						//boost::property_tree::ptree s_Request;
						//s_Request.add("Username", "kiwidog");
						//s_Request.add("Password", "password");

						//std::ostringstream s_Stream;

						//try
						//{
						//	write_json(s_Stream, s_Request, true);
						//}
						//catch (std::exception p_Exception)
						//{
						//	WriteLog("write_json failed (%s).", p_Exception.what());
						//	return false;
						//	// ignored
						//}

						//http::client s_Client;
						//http::client::request s_PostRequest("http://localhost:33642/api/Users/Login");

						//s_PostRequest << body(s_Stream.str());

						//s_PostRequest << header("Connection", "close");

						//http::client::response s_Response = s_Client.post(s_PostRequest);

						//std::string s_Out = body(s_Response);

						//WriteLog("Body Response: %s", s_Out.c_str());

						//// TODO: Validate
						//p_RetVal = CefV8Value::CreateBool(true);
						//return true;
					}
				};
			}
		}
	}
}
