#include "Connector.hh"
#include "Logger.hh"
#include "SocketHelp.hh"
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>

int g_sockfd = -1;
static std::string domain;

Connector::Connector(const InetAddress& serverAddr):m_serverAddr(serverAddr)
{
	LOG_DEBUG << "Ctor[" /*<< this*/ << "]";
	//connect();
}

Connector::Connector(const std::string Domain):m_serverAddr(InetAddress("0.0.0.0", 0))
{
	domain = Domain;
}

Connector::~Connector()
{
	LOG_DEBUG << "Dtor[" /*<< this*/ << "]";
}


void Connector::connect()
{

	char ip[32] = {0};
	while(1)
	{
		struct hostent* phost = NULL;
	 
		phost = gethostbyname(domain.c_str());
	    if (NULL == phost)
	    {
	    	LOG_ERROR << "HttpUrlToIp(): gethostbyname error : " << errno << " : "<< strerror(errno) << " continue.";
	    	sockets::delaySecond(1);
	    	continue;
	    }
	        
	    inet_ntop(phost->h_addrtype,  phost->h_addr, ip, 17);
		m_serverAddr = InetAddress(ip, 80);

		LOG_DEBUG << "Connector::Connector() gethostbyname Successful";

		int sockfd = sockets::createSocket(m_serverAddr.family());
		if(sockfd < 0) LOG_SYSERR << "Connector::connect() : createSocket error";
		int ret = sockets::connect(sockfd, m_serverAddr.getSockAddr());
		LOG_DEBUG << "sockfd : " << sockfd << "sockets::connect ret : " << ret ; 
		int savedErrno = (ret == 0) ? 0 : errno;

		switch (savedErrno)
		{
			case 0:
			case EINPROGRESS:
			case EINTR:
			case EISCONN:
			  //connecting(sockfd);
			  g_sockfd = sockfd;
			  LOG_INFO << "Connector::connect() sockfd : " << sockfd << " Successful";
			  break;
			default :
			  LOG_ERROR << "Connect Error ";
  			  sockets::delaySecond(1);
			  continue;
		}

		break;
	}

	LOG_DEBUG << "Connector::Connector() end";

/*
	int sockfd = sockets::createSocket(m_serverAddr.family());
	if(sockfd < 0) LOG_SYSERR << "Connector::connect() : createSocket error";
	int ret = sockets::connect(sockfd, m_serverAddr.getSockAddr());
	LOG_DEBUG << sockfd << ret ; 
	int savedErrno = (ret == 0) ? 0 : errno;
	switch (savedErrno)
	{
		case 0:
		case EINPROGRESS:
		case EINTR:
		case EISCONN:
		  //connecting(sockfd);
		  g_sockfd = sockfd;
		  LOG_INFO << "Connector::connect() sockfd : " << sockfd << " Successful";
		  break;

		case EAGAIN:
		case EADDRINUSE:
		case EADDRNOTAVAIL:
		case ECONNREFUSED:
		case ENETUNREACH:
		  //retry(sockfd);
		  LOG_SYSFATAL << "connect retry undefine : " << savedErrno;
		  break;

		case EACCES:
		case EPERM:
		case EAFNOSUPPORT:
		case EALREADY:
		case EBADF:
		case EFAULT:
		case ENOTSOCK:
		  sockets::close(sockfd);
		  LOG_SYSFATAL << "connect error in Connector::startInLoop " << savedErrno;
		  break;

		default:
		  sockets::close(sockfd);
		  LOG_SYSFATAL << "Unexpected error in Connector::startInLoop " << savedErrno;
		  // connectErrorCallback_();
		  break;
	}
*/
}
