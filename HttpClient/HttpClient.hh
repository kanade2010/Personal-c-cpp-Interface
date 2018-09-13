#ifndef _HTTPCLIENT_HH
#define _HTTPCLIENT_HH

#include "TcpClient.hh"
#include "StringPiece.hh"
#include "HttpUrl.hh"

class HttpClient
{
public:
	enum HttpRequestMethod{
		GET = 0,
		POST
	};
	HttpClient(std::string httpUrl);
	~HttpClient();

	void connect() {m_tcpClient.connect();}
	void TEST(const std::string path,const std::string content);
	void setRequestMethod(std::string method);
	void setRequestProperty(std::string key, std::string value);

	void clearStream() {m_stream.str("");}
	std::string strStream() const { return m_stream.str(); };

private:
	HttpUrl m_httpUrl;
	TcpClient m_tcpClient;
	std::stringstream m_stream;
};


#endif