#ifndef _HTTPURL_HH
#define _HTTPURL_HH
#include "StringPiece.hh"
#include "Logger.hh"
#include <netdb.h>
#include <arpa/inet.h>
#include <regex>
#include "InetAddress.hh"

class HttpUrl
{
public:
	HttpUrl(std::string& httpUrl)
	:m_httpUrl(httpUrl),
	 m_smatch(detachHttpUrl())
	{

	}
	~HttpUrl(){};

	enum HttpUrlMatch
	{
		URL = 0,
		HOST = 3,
		URI = 4
	};

	std::smatch detachHttpUrl() const
	{
		std::regex httpExpression("((http|https)?://)?([0-9\\.a-zA-Z]*)/?(.*)?");
		std::smatch result;

		if(regex_match(m_httpUrl, result, httpExpression))
		{
			LOG_DEBUG << "detachHttpUrl() url :" << result[URL].str().c_str();
			return result;
		}
		else
		{
			LOG_FATAL << "detachHttpUrl() failed format error";
		}

	}

	bool HttpUrlToIp(const std::string& host, char* ip) const
	{
		struct hostent* phost = NULL;
 
		phost = gethostbyname(host.c_str());
	    if (NULL == phost)
	    {
	    	LOG_ERROR << "HttpUrlToIp(): gethostbyname error";
	    	return false;
	    	//LOG_SYSERR << "urlToIp(): gethostbyname error";
	    }
	        
	    inet_ntop(phost->h_addrtype,  phost->h_addr, ip, 17);

	    return true;
	}

	InetAddress toInetAddr() const
	{
		std::string host = getHttpUrlSubSeg(HOST);
		char ip[32];
		if(!HttpUrlToIp(host, ip)) LOG_SYSERR << "toInetAddr(): hostnameToIp error";
		LOG_DEBUG << "HttpUrlToIp : " << ip;
		InetAddress servAddr(ip, 80);
		return servAddr;
	}

	std::string getHttpUrlSubSeg(HttpUrlMatch sub = HOST) const{ return m_smatch[sub].str(); }

private:
	std::string m_httpUrl;
	std::smatch m_smatch;
};

#endif