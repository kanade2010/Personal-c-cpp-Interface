#include "HttpClient.hh"
#include "InetAddress.hh"
#include <string>
#include <iostream>
#include "SocketHelp.hh"

HttpClient::HttpClient(std::string httpUrl)
	:m_httpUrl(httpUrl),
	m_tcpClient(m_httpUrl.toInetAddr()),
	m_stream()
{

}

HttpClient::~HttpClient()
{

}

void HttpClient::TEST(const std::string path,const std::string content)
{
	//GET请求方式
	//std::stringstream m_stream;
	m_stream << "GET " << path << "?" << content;
	m_stream << " HTTP/1.0\r\n";
	m_stream << "Host: " << m_httpUrl.getHttpUrlSubSeg(HttpUrl::HOST) << "\r\n";
	m_stream <<"Connection: Keep-Alive\r\n\r\n";

	std::cout << "\n\n" << m_stream.str() << std::endl;

	std::cout << m_httpUrl.getHttpUrlSubSeg(HttpUrl::URI) << std::endl;

	m_tcpClient.connect();

	sockets::write(g_sockfd, m_stream.str().c_str(), m_stream.str().size());
	char buf[4096] = {0};
	while(sockets::read(g_sockfd, buf, 4096) > 0)
		std::cout << buf << std::endl;

	std::cout << "\n\n-----------------------------------------------------\n\n" << std::endl;

/*
	sockets::write(g_sockfd, m_stream.str().c_str(), m_stream.str().size());
	sockets::read(g_sockfd, buf, 4096);
	std::cout << buf << std::endl;
*/

}

const std::map<std::string, int>::value_type init_value[] =
{
	std::map<std::string, int>::value_type( "GET", HttpClient::GET),

	std::map<std::string, int>::value_type( "POST", HttpClient::POST)
};

const static std::map<std::string, int> kRequestMethodMap(init_value, init_value+2);

void HttpClient::setRequestMethod(std::string method)
{
	switch(kRequestMethodMap.at(method))
	{
		case HttpClient::GET :
			m_stream << "GET " << "/" << m_httpUrl.getHttpUrlSubSeg(HttpUrl::URI) << " HTTP/1.0\r\n";
			LOG_DEBUG << m_stream.str().c_str();
			break;
		case HttpClient::POST :
			m_stream << "POST " << m_httpUrl.getHttpUrlSubSeg(HttpUrl::URI) << " HTTP/1.0\r\n";
			LOG_DEBUG << m_stream.str().c_str();
			break;
		default :
			LOG_ERROR << "No such Method";
			break;
	}

	m_stream << "Host: " << m_httpUrl.getHttpUrlSubSeg(HttpUrl::HOST) << "\r\n";
}

void HttpClient::setRequestProperty(std::string key, std::string value)
{
	m_stream << key << ": " << value << "\r\n";
}
