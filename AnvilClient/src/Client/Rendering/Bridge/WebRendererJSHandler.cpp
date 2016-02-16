#include "WebRendererJSHandler.hpp"

using namespace Anvil::Client::Rendering::Bridge;

WebRendererJSHandler::WebRendererJSHandler(std::string p_HandlerName)
	: m_Name(p_HandlerName)
{
	m_Object = CefV8Value::CreateObject(nullptr);
}

bool WebRendererJSHandler::Execute(const CefString& p_Name, CefRefPtr<CefV8Value> p_Object, const CefV8ValueList& p_Arguments, CefRefPtr<CefV8Value>& p_RetVal, CefString& p_Exception)
{
	for (auto& l_Pair : m_Functions)
	{
		auto l_Name = l_Pair.first;
		auto l_Function = l_Pair.second;
		if (!l_Function)
			continue;

		if (p_Name == l_Name)
			return l_Function(p_Arguments, p_RetVal);
	}
	return false;
}

bool WebRendererJSHandler::AddFunction(std::string p_FunctionName, BridgeFunc p_Function)
{
	for (auto& l_Pair : m_Functions)
		if (l_Pair.first == p_FunctionName)
			return false;

	// Add our function name to native function
	m_Functions.push_back(make_pair(p_FunctionName, p_Function));

	// Create a new javascript function within this handler
	auto s_JSFunction = CefV8Value::CreateFunction(p_FunctionName, this);
	
	// Adds this to the handler object (aka "handlername"."functionname")
	m_Object->SetValue(p_FunctionName, s_JSFunction, V8_PROPERTY_ATTRIBUTE_NONE);

	// Add the newly created JS function to the function name, JS function map
	m_CefFunctions.push_back(make_pair(p_FunctionName, s_JSFunction));

	return true;
}

std::string WebRendererJSHandler::GetName()
{
	return m_Name;
}

CefRefPtr<CefV8Value> WebRendererJSHandler::GetHandlerObject()
{
	return m_Object;
}