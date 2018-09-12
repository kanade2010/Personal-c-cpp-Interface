#ifndef _URL_HH
#define _URL_HH
#include "StringPiece.hh"
#include "Logger.hh"
#include <netdb.h>
#include <arpa/inet.h>
#include <regex>
#include "InetAddress.hh"

class HttpUrl
{
public:
	HttpUrl(std::string& httpUrl):m_httpUrl(httpUrl){};
	~HttpUrl(){};


	std::string getHttpUrlDomain() const
	{
		std::regex httpExpression("((http|https)?://)?([0-9\\.a-z]*)(/.*)?");
		std::smatch result;

		if(regex_match(m_httpUrl, result, httpExpression))
		{
			LOG_DEBUG << "getHttpUrlDomain() url :" << result[3].str().c_str();
			return result[3].str();
		}
		else
		{
			LOG_DEBUG << "getHttpUrlDomain() failed";
			return "";
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
		std::string host = getHttpUrlDomain();
		char ip[32];
		if(!HttpUrlToIp(host, ip)) LOG_SYSERR << "toInetAddr(): hostnameToIp error";
		LOG_DEBUG << "HttpUrlToIp : " << ip;
		InetAddress servAddr(ip, 80);
		return servAddr;
	}


private:
	std::string m_httpUrl;
};

#endif