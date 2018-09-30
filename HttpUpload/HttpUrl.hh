#ifndef _HTTPURL_HH
#define _HTTPURL_HH
#include "StringPiece.hh"
#include "Logger.hh"
#include <netdb.h>
#include <arpa/inet.h>
#include <regex>
#include "InetAddress.hh"
#include "SocketHelp.hh"
#include <assert.h>
#include <vector>

class HttpUrl
{
public:
	HttpUrl(std::string& httpUrl)
	:m_httpUrl(httpUrl),
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
/*
		std::regex httpExpression("((http|https)?://)?([a-zA-Z0-9\\.]+)/?(services/publicSoftService|pointdown\?versionDetailId=22807)?");
		std::smatch result;

		if(regex_match(m_httpUrl, result, httpExpression))
		{
			LOG_DEBUG << "detachHttpUrl() host :" << result[0].str().c_str();
			LOG_DEBUG << "detachHttpUrl() host :" << result[1].str().c_str();
			LOG_DEBUG << "detachHttpUrl() host :" << result[2].str().c_str();
			LOG_DEBUG << "detachHttpUrl() host :" << result[3].str().c_str();
			LOG_DEBUG << "detachHttpUrl() url :" << result[4].str().c_str();
			return result;
		}
		else
		{
			LOG_FATAL << "detachHttpUrl() failed format error";
		}
*/
	}

	bool HttpUrlToIp(const std::string& host, char* ip) const
	{
		struct hostent* phost = NULL;
 
		phost = gethostbyname(host.c_str());
	    if (NULL == phost)
	    {
	    	LOG_ERROR << "HttpUrlToIp(): gethostbyname error : " << errno << " : "<< strerror(errno);
	    	return false;
	    	//LOG_SYSERR << "urlToIp(): gethostbyname error";
	    }
	        
	    inet_ntop(phost->h_addrtype,  phost->h_addr, ip, 17);

	    return true;
	}

	/*InetAddress toInetAddr() const
	{
		std::string host = getHttpUrlSubSeg(HOST);
		char ip[32];
		while(!HttpUrlToIp(host, ip))
		{
		  switch(errno){
		    case ETIMEDOUT:
		      LOG_DEBUG << "Connection Time Out ReConnection ...";
		      break;
		    default:
		      LOG_FATAL << "toInetAddr(): hostnameToIp error";
			  break;
			}
			sockets::delaySecond(1);
		}
		LOG_DEBUG << "HttpUrlToIp : " << ip;
		InetAddress servAddr(ip, 80);
		return servAddr;
	}*/

	std::string domain() const
	{
		return getHttpUrlSubSeg(HOST);
	}

	std::string getHttpUrlSubSeg(HttpUrlMatch sub = HOST) const{ return m_smatch[sub]; }

private:
	std::string m_httpUrl;
	std::vector<std::string> m_smatch;
};

#endif