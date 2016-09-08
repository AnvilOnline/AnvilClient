#include <Utils/Logger.hpp>

#include "ArchClient.hpp"/*
#include <cpprest/http_client.h>
#include <cpprest/json.h>*/
#include <iostream>

using namespace Anvil::Client::Arch;

ArchClient::ArchClient(std::wstring p_Host) :
	m_HostNode(p_Host),
	m_Username(L"unconnected"),
	m_Identity(GUID_NULL),
	m_NodeToken(GUID_NULL),
	m_Status(ACS_Disconnected)
{

}

bool ArchClient::Login(std::wstring p_Username, std::wstring p_Password)
{
	return false;
//	web::http::client::http_client s_Client(m_HostNode);
//
//	web::json::value s_PostParameters = web::json::value::object();
//
//	s_PostParameters[L"Username"] = web::json::value::string(p_Username);
//	s_PostParameters[L"Password"] = web::json::value::string(p_Password);
//
//
//	web::http::http_request s_Request;
//	s_Request.set_request_uri(L"/api/Users/Login");
//	s_Request.set_method(web::http::methods::POST);
//	s_Request.set_body(s_PostParameters);
//
//	auto s_Response = s_Client.request(s_Request).get();
//
//	auto s_ResponseResult = s_Response.extract_json().get();
//
//	if (!s_ResponseResult.has_field(L"result"))
//		return false;
//
//	auto s_ResultValue = s_ResponseResult.at(L"result");
//
//	if (!s_ResultValue.is_boolean())
//		return false;
//
//	auto s_Result = s_ResultValue.as_bool();
//
//	WriteLog("Result: %s", s_Result ? "successful" : "unsuccessful");
//
//	/*Anvil::Client::Rendering::WebRenderer::GetInstance()->ShowNotification("Login", s_Result ? "successful" : "unsuccessful");
//*/
//	return s_Result;
}
