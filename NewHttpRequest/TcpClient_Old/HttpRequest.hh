#ifndef _HTTP_REQUEST_HH
#define _HTTP_REQUEST_HH

#include <netdb.h>
#include <arpa/inet.h>
#include <algorithm>
#include <vector>
#include <string>
#include <assert.h>
#include <string.h>
#include <sstream>
#include <map>
#include <memory>
#include "Logger.hh"
#include "Buffer.hh"
#include "TcpClient.hh"
#include "InetAddress.hh"
#include "CallBacks.hh"

class HttpUrl
{
public:
	HttpUrl(std::string url)
	:m_httpUrl(url),
	 m_smatch(detachHttpUrl())
	{
		LOG_DEBUG << "URL : " << m_httpUrl;
	}
	~HttpUrl(){};

	enum HttpUrlMatch
	{
		URL = 0,
		HOST = 1,
		URI = 2
	};

	std::vector<std::string> detachHttpUrl() const
	{
	  std::vector<std::string> v;
	  std::string::size_type pos1, pos2;
	  pos2 = m_httpUrl.find('/');
	  assert(std::string::npos != pos2);
	  pos1 = pos2 + 2;
	  pos2 = m_httpUrl.find('/', pos1);
	  assert(std::string::npos != pos2);
	  v.push_back(m_httpUrl);
	  v.push_back(m_httpUrl.substr(pos1, pos2 - pos1));
	  v.push_back(m_httpUrl.substr(pos2 + 1));
	  LOG_DEBUG << "detachHttpUrl() url :" << v[0];
	  LOG_DEBUG << "detachHttpUrl() host :" << v[1];
	  LOG_DEBUG << "detachHttpUrl() uri :" << v[2];
	  return v;
	}

	std::string toIp() const
	{
		struct hostent* phost = NULL;
		char ip[32] = {0};

		phost = gethostbyname(domain().c_str());
    if (NULL == phost)
    {
    	LOG_ERROR << "HttpUrlToIp(): gethostbyname error : " << errno << " : "<< strerror(errno);
    	return "";
    	//LOG_SYSERR << "urlToIp(): gethostbyname error";
    }

    inet_ntop(phost->h_addrtype,  phost->h_addr, ip, sizeof ip);

    return ip;
	}

	std::string domain() const
	{
		return getHttpUrlSubSeg(HOST);
	}

	std::string uri() const
	{
		return getHttpUrlSubSeg(URI);
	}

private:
	std::string getHttpUrlSubSeg(HttpUrlMatch sub = HOST) const{ return m_smatch[sub]; }

	std::string m_httpUrl;
	std::vector<std::string> m_smatch;
};


class EventLoop;

class HttpRequest
{
public:
	enum HttpRequestMethod{
		GET = 0,
		POST
	};
	HttpRequest(EventLoop* loop, const InetAddress& servAddr);
	~HttpRequest();

	const char* peek() { return m_buffer.peek(); }

	void setRequestMethod(const std::string &method, const HttpUrl& url);
	void setRequestProperty(const std::string &key, const std::string &value);
	void setRequestBody(const std::string &content);


	int getResponseCode() const {
		assert(m_haveHandleHead);
		return m_code;
	}

	std::string getResponseProperty(const std::string& key) const {
		assert(m_haveHandleHead);
		return m_ackProperty.at(key);
	}

	std::string getResponseContent() {
		assert(m_haveHandleHead);
		return std::string(m_buffer.peek(), m_buffer.readableBytes());
	}

	void connect() { p_tcpClient->start(); } ;
	void send();

	void close();//{ sockets::close(m_sockfd); }

private:
	void newConnetion(const TcpConnectionPtr& conn);
  void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c);
	void handleRead(const TcpConnectionPtr& conn, Buffer* buffer, ssize_t len);
	void downloadFile(const TcpConnectionPtr& conn, Buffer* buffer, ssize_t len);

  EventLoop* p_loop;
  std::unique_ptr<TcpClient> p_tcpClient;
	Buffer m_buffer;
	int m_code;
	bool m_haveHandleHead;
	std::map<std::string, std::string> m_ackProperty;

	//std::stringstream m_stream;

};

#endif