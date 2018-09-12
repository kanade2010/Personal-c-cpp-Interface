#include "Connector.hh"
#include "Logger.hh"
#include "SocketHelp.hh"
#include <errno.h>

Connector::Connector(const InetAddress& serverAddr):m_serverAddr(serverAddr)
{
	LOG_DEBUG << "Ctor[" /*<< this*/ << "]";
	//connect();
}

Connector::~Connector()
{
	LOG_DEBUG << "Dtor[" /*<< this*/ << "]";
}


void Connector::connect()
{
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
		  LOG_INFO << "Connector::connect() Successful";
		  break;

		case EAGAIN:
		case EADDRINUSE:
		case EADDRNOTAVAIL:
		case ECONNREFUSED:
		case ENETUNREACH:
		  //retry(sockfd);
		  break;

		case EACCES:
		case EPERM:
		case EAFNOSUPPORT:
		case EALREADY:
		case EBADF:
		case EFAULT:
		case ENOTSOCK:
		  LOG_SYSERR << "connect error in Connector::startInLoop " << savedErrno;
		  sockets::close(sockfd);
		  break;

		default:
		  LOG_SYSERR << "Unexpected error in Connector::startInLoop " << savedErrno;
		  sockets::close(sockfd);
		  // connectErrorCallback_();
		  break;
	}

}
