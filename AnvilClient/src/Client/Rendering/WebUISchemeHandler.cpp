#include "WebUISchemeHandler.hpp"

#define _WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN

#include <boost/network/uri.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

#include <Utils/Logger.hpp>

#pragma comment(lib, "cppnetlib-uri")

using namespace Anvil::Client::Rendering;

std::unordered_map<int64_t, std::unordered_map<std::string, std::string>> WebUISchemeHandler::m_CachedData;
std::mutex WebUISchemeHandler::m_CachedDataMutex;

WebUISchemeHandler::WebUISchemeHandler(bool p_Main, CefRefPtr<CefFrame> p_Frame)
	: m_Main(p_Main), m_Frame(p_Frame)
{
	m_RequestedLength = 0;
	m_Offset = 0;
	m_Partial = false;
}

WebUISchemeHandler::~WebUISchemeHandler()
{
	m_TempData.clear();
	m_TempFileName.clear();
	m_ContentType.clear();
	m_RequestedLength = 0;
	m_Offset = 0;
	m_Partial = false;
	m_ShouldCache = false;
}

void WebUISchemeHandler::ProcessRequestInternal(CefRefPtr<CefRequest> p_Request, CefRefPtr<CefCallback> p_Callback)
{
	boost::lock_guard<WebUISchemeHandler> s_Guard(*this);

	CefRequest::HeaderMap s_Headers;
	p_Request->GetHeaderMap(s_Headers);

	WriteLog("Processing request: %s", p_Request->GetURL().ToString().c_str());

	// [scheme:][//authority][path][?query][#fragment]
	boost::network::uri::uri s_RequestURI(p_Request->GetURL().ToString());

	if (!s_RequestURI.is_valid())
	{
		p_Callback->Cancel();
		return;
	}

	if (s_RequestURI.scheme() != "ao")
	{
		p_Callback->Cancel();
		return;
	}

	std::string s_FinalPath = s_RequestURI.path();

	if (s_FinalPath.size() == 0)
		s_FinalPath = "/";

	if (s_FinalPath.substr(s_FinalPath.size() - 1) == "/")
		s_FinalPath += "index.html";

	std::string s_Host = s_RequestURI.host();

	p_Callback->Continue();
}

bool WebUISchemeHandler::ProcessRequest(CefRefPtr<CefRequest> p_Request, CefRefPtr<CefCallback> p_Callback)
{
	// TODO: We shouldn't spawn a thread every single time, but do this on a single thread.
	boost::thread s_Thread(&WebUISchemeHandler::ProcessRequestInternal, this, p_Request, p_Callback);

	return true;
}

void WebUISchemeHandler::GetResponseHeaders(CefRefPtr<CefResponse> p_Response, int64& p_ResponseLength, CefString& p_RedirectURL)
{
	p_ResponseLength = m_RequestedLength;
	p_Response->SetStatus(m_Partial ? 206 : 200);
	p_Response->SetMimeType(m_ContentType.size() == 0 ? "application/octet-stream" : m_ContentType);

	CefResponse::HeaderMap s_Headers;

	if (m_ShouldCache)
		s_Headers.insert(std::make_pair("Cache-Control", "public, max-age=0"));

	s_Headers.insert(std::make_pair("Content-Length", boost::lexical_cast<std::string>(m_RequestedLength).c_str()));

	// Is this a partial response?
	if (m_Partial)
	{
		p_Response->SetStatusText("Partial Content");
		s_Headers.insert(std::make_pair("Content-Range", str(boost::format("bytes %s/%d") % m_RequestedRange.c_str() % m_TempData.size()).c_str()));
	}

	p_Response->SetHeaderMap(s_Headers);
}

bool WebUISchemeHandler::ReadResponse(void* p_DataOut, int p_BytesToRead, int& p_BytesRead, CefRefPtr<CefCallback> p_Callback)
{
	// Do we have enough data for this?
	if (static_cast<size_t>(m_Offset) >= m_TempData.size())
		return false;

	auto s_BytesToRead = static_cast<size_t>(p_BytesToRead) > (m_TempData.size() - m_Offset) ? m_TempData.size() : p_BytesToRead;

	memcpy(p_DataOut, m_TempData.data() + m_Offset, s_BytesToRead);
	m_Offset += s_BytesToRead;

	p_BytesRead = s_BytesToRead;

	return true;
}

bool WebUISchemeHandler::CanGetCookie(const CefCookie& p_Cookie)
{
	return true;
}

bool WebUISchemeHandler::CanSetCookie(const CefCookie& p_Cookie)
{
	return true;
}

void WebUISchemeHandler::Cancel()
{
}

void WebUISchemeHandler::ExpireCache(int64_t p_Frame)
{
	m_CachedDataMutex.lock();

	auto s_Iterator = m_CachedData.find(p_Frame);

	if (s_Iterator != m_CachedData.end())
		m_CachedData.erase(s_Iterator);

	m_CachedDataMutex.unlock();
}