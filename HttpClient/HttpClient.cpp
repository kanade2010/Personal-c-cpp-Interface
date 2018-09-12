#include "HttpClient.hh"
#include "InetAddress.hh"

HttpClient::HttpClient(std::string httpUrl)
	:m_httpUrl(httpUrl),
	m_tcpClient(m_httpUrl.toInetAddr())
{

}

HttpClient::~HttpClient()
{

}

void HttpClient::GET()
{
	m_tcpClient.connect();
}