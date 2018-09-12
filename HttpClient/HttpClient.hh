#ifndef _HTTPCLIENT_HH
#define _HTTPCLIENT_HH

#include "TcpClient.hh"
#include "StringPiece.hh"
#include "HttpUrl.hh"

class HttpClient
{
public:
	HttpClient(std::string httpUrl);
	~HttpClient();

	void GET();

private:
	HttpUrl m_httpUrl;
	TcpClient m_tcpClient;
};


#endif