#ifndef _SOCKETHELP_HH
#define _SOCKETHELP_HH

#include <arpa/inet.h>

namespace sockets
{

///
/// Creates a non-blocking socket file descriptor,
/// abort if any error.
int createSocket(sa_family_t family);

int  connect(int sockfd, const struct sockaddr* addr);
void bindOrDie(int sockfd, const struct sockaddr* addr);
void listenOrDie(int sockfd);

ssize_t read(int sockfd, void *buf, size_t count);
ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);
ssize_t write(int sockfd, const void *buf, size_t count);
void close(int sockfd);
void sockets::shutdownWrite(int sockfd);

void fromIpPort(const char* ip, uint16_t port,
                struct sockaddr_in* addr);

int getSocketError(int sockfd);
void delaySecond(int sec);
//const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr)
//const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr);

}

#endif 