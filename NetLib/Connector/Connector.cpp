#include <assert.h>

#include "SocketHelp.hh"
#include "Connector.hh"
#include "Logger.hh"

Connector::Connector(EventLoop* loop, const InetAddress& serverAddr)
  :p_loop(loop),
  m_serverAddr(serverAddr)
{

}

Connector::~Connector()
{

}


void Connector::connect()
{
  int sockfd = sockets::createNonblockingOrDie(m_serverAddr.family());
  int ret = sockets::connect(sockfd, m_serverAddr.getSockAddr());
  int savedErrno = (ret == 0) ? 0 : errno;

  switch(savedErrno)
  {
    case 0:
    case EINPROGRESS:
    case EINTR:
    case EISCONN:
      connecting(sockfd);
      break;

    case EAGAIN:
    case EADDRINUSE:
    case EADDRNOTAVAIL:
    case ECONNREFUSED:
    case ENETUNREACH:
      //retry(sockfd);
      LOG_SYSERR << "reSave Error. " << savedErrno;
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

void Connector::connecting(int sockfd)
{
  assert(!p_channel);
  p_channel.reset(new Channel(p_loop, sockfd));
  p_channel->setWriteCallBack(std::bind(&Connector::handleWrite, this));

  p_channel->enableWriting();
}

void Connector::handleWrite()
{
  LOG_TRACE << "Connector::handleWrite ";
  m_newConnectionCallBack(p_channel->fd());
}