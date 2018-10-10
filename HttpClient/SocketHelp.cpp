#include "SocketHelp.hh"
#include "Logger.hh"
#include "Edian.hh"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/uio.h>  // readv

int sockets::createSocket(sa_family_t family){ 
  // Call "socket()" to create a (family) socket of the specified type.
  // But also set it to have the 'close on exec' property (if we can)
	
	int sock;

	//CLOEXEC，即当调用exec（）函数成功后，文件描述符会自动关闭。
	//在以往的内核版本（2.6.23以前）中，需要调用 fcntl(fd, F_SETFD, FD_CLOEXEC) 来设置这个属性。
	//而新版本（2.6.23开始）中，可以在调用open函数的时候，通过 flags 参数设置 CLOEXEC 功能，
#ifdef SOCK_CLOEXEC
  sock = socket(family, SOCK_STREAM|SOCK_CLOEXEC, 0);
  if (sock != -1 || errno != EINVAL) return sock;
  // An "errno" of EINVAL likely means that the system wasn't happy with the SOCK_CLOEXEC; fall through and try again without it:
#endif

  sock = socket(family, SOCK_STREAM, 0);

#ifdef FD_CLOEXEC
  if (sock != -1) fcntl(sock, F_SETFD, FD_CLOEXEC);
#endif
  return sock;
}

int sockets::connect(int sockfd, const struct sockaddr* addr)
{
  return ::connect(sockfd, addr, sizeof(struct sockaddr));
}

void sockets::fromIpPort(const char* ip, uint16_t port,
                         struct sockaddr_in* addr)
{
  addr->sin_family = AF_INET;
  addr->sin_port = hostToNetwork16(port);
  if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
  {
    LOG_SYSERR << "sockets::fromIpPort";
  }
}

ssize_t sockets::read(int sockfd, void *buf, size_t count)
{
  return ::read(sockfd, buf, count);
}

ssize_t sockets::readv(int sockfd, const struct iovec *iov, int iovcnt)
{
  return ::readv(sockfd, iov, iovcnt);
}

ssize_t sockets::write(int sockfd, const void *buf, size_t count)
{
  return ::write(sockfd, buf, count);
}

void sockets::close(int sockfd)
{
  if (::close(sockfd) < 0)
  {
    LOG_SYSERR << "sockets::close";
  }
}

void sockets::delaySecond(int sec)
{
  struct  timeval tv;
  tv.tv_sec = sec;
  tv.tv_usec = 0;
  select(0, NULL, NULL, NULL, &tv);
}


/*
const struct sockaddr* sockets::sockaddr_cast(const struct sockaddr_in* addr)
{
  return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
}

const struct sockaddr* sockets::sockaddr_cast(const struct sockaddr_in* addr)
{
  return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
}*/