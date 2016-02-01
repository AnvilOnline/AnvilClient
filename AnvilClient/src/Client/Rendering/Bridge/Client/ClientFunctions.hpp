#pragma once
#include <include/cef_v8.h>
#include <Utils/Logger.hpp>
#include <Client/AnvilClient.hpp>
#include <Utils/Util.hpp>
#include <SDK/Unsorted/simulation_sandbox_engine_globals_definition.h>

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

					if (p_Name == "LoadMap")
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

						return true;
					}

					if (p_Name == "GetPtr")
					{
						uint32_t s_BaseAddress = 0, s_BaseSize = 0;
						if (!Utils::Util::GetExecutableInfo(s_BaseAddress, s_BaseSize))
							return false;

						auto s_SandboxEngineGlobalsAddress = *reinterpret_cast<uint32_t*>(s_BaseAddress + 0x4FF8B9C);
						if (!s_SandboxEngineGlobalsAddress || s_SandboxEngineGlobalsAddress == -1)
							return false;

						auto s_SandboxEngineGlobals = reinterpret_cast<simulation_sandbox_engine_globals_definition*>(s_SandboxEngineGlobalsAddress);

						WriteLog("SandboxEngineGloabls: %s.", s_SandboxEngineGlobals->getName());
						return true;
					}
					return false;
				}

			protected:

				void Quit()
				{
					// TODO: Implement AnvilClient::Shutdown to ensure all resources get freed properly.
					TerminateProcess(GetCurrentProcess(), 0);
				}

				IMPLEMENT_REFCOUNTING(ClientFunctionsHandler);
			};
		}
	}
}