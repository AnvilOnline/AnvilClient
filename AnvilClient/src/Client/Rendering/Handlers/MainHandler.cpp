#include <boost/network/uri.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>

#include "MainHandler.hpp"
#include <Client/Rendering/WebUI.hpp>

using namespace Anvil::Client::Rendering;
using namespace Handlers;

MainHandler::MainHandler()
	: m_Frame(nullptr)
{

}

bool MainHandler::OnQuery(CefRefPtr<CefBrowser> p_Browser, CefRefPtr<CefFrame> p_Frame, int64 p_QueryID, const CefString& p_Request, bool p_Persistent, CefRefPtr<Callback> p_Callback)
{
	return true;
}

std::string MainHandler::GetFrameName(CefRefPtr<CefFrame> p_Frame)
{
	boost::network::uri::uri s_RequestURI(p_Frame->GetURL().ToString());

	if (!s_RequestURI.is_valid())
		return "";

	if (s_RequestURI.scheme() != "vu")
		return "";

	std::string s_Host = s_RequestURI.host();

	std::string s_LowerName(s_Host);
	std::transform(s_LowerName.begin(), s_LowerName.end(), s_LowerName.begin(), ::tolower);

	if (s_LowerName == "main" || s_LowerName == "game")
		return "";

	return s_LowerName;
}

std::string MainHandler::EscapeString(const std::string& p_String)
{
	// TODO: Re-implement
	return "";
}