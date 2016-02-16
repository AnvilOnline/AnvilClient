#pragma once
#include <include/cef_v8.h>

namespace Anvil
{
	namespace Client
	{
		namespace Rendering
		{
			class NodeInformationHandler : public CefV8Handler
			{
			public:
				// TODO: This will be a part of the Cirrus SDK
				class CirrusNodeInformation
				{
					// TODO: Unghetto this
				public:
					std::string m_Name;
					std::string m_IpAddress;
					uint64_t m_UserCount;
					uint64_t m_MaxUserCount;
					bool m_Password;
				};

				CirrusNodeInformation m_NodeInformation;

				virtual bool Execute(const CefString& p_Name, CefRefPtr<CefV8Value> p_Object, const CefV8ValueList& p_Arguments, CefRefPtr<CefV8Value>& p_RetVal, CefString& p_Exception) override
				{
					if (p_Name == "NodeGetName")
					{
						p_RetVal = CefV8Value::CreateString(NodeGetName());
						return true;
					}

					if (p_Name == "NodeGetAddress")
					{
						p_RetVal = CefV8Value::CreateString(NodeGetAddress());
						return true;
					}

					if (p_Name == "NodeGetUserCount")
					{
						p_RetVal = CefV8Value::CreateUInt(NodeGetUserCount());
						return true;
					}

					if (p_Name == "NodeGetMaxUserCount")
					{
						p_RetVal = CefV8Value::CreateUInt(NodeGetMaxUserCount());
						return true;
					}

					if (p_Name == "NodeHasPassword")
					{
						p_RetVal = CefV8Value::CreateBool(NodeHasPassword());
						return true;
					}

					return false;
				}

				std::string& NodeGetName()
				{
					return m_NodeInformation.m_Name;
				}

				std::string& NodeGetAddress()
				{
					return m_NodeInformation.m_IpAddress;
				}

				uint64_t NodeGetUserCount()
				{
					return m_NodeInformation.m_UserCount;
				}

				uint64_t NodeGetMaxUserCount()
				{
					return m_NodeInformation.m_MaxUserCount;
				}

				bool NodeHasPassword()
				{
					return m_NodeInformation.m_Password;
				}

			protected:
				IMPLEMENT_REFCOUNTING(NodeInformationHandler);
			};
		}
	}
}