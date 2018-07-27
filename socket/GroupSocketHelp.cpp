#include "GroupSocketHelp.hh"
#include <fcntl.h>
#include "debug.h"

#define TAG "SocketInterface"

// By default, use INADDR_ANY for the sending and receiving interfaces:
netAddressBits SendingInterfaceAddr = INADDR_ANY;
netAddressBits ReceivingInterfaceAddr = INADDR_ANY;

void delaySecond(int sec)
{
	struct  timeval tv;
	tv.tv_sec = sec;
	tv.tv_usec = 0;
	select(0, NULL, NULL, NULL, &tv);
}

Boolean makeSocketNonBlocking(int sock){
  //F_GETFL Return (as the function result) the file access mode and the file status flags; arg is ignored.
  int curFlags = fcntl(sock, F_GETFL, 0);
  return fcntl(sock, F_SETFL, curFlags|O_NONBLOCK) >= 0;
}

static void makeSocketAddress(struct sockaddr_in &addr, IpAddrBits ip, portNumBits port)
{
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ip;
	addr.sin_port = htons(port);
}

static int createSocket(int type){ 
  // Call "socket()" to create a (IPv4) socket of the specified type.
  // But also set it to have the 'close on exec' property (if we can)
	
	int sock;

	//CLOEXEC，即当调用exec（）函数成功后，文件描述符会自动关闭。
	//在以往的内核版本（2.6.23以前）中，需要调用 fcntl(fd, F_SETFD, FD_CLOEXEC) 来设置这个属性。
	//而新版本（2.6.23开始）中，可以在调用open函数的时候，通过 flags 参数设置 CLOEXEC 功能，
#ifdef SOCK_CLOEXEC
  sock = socket(AF_INET, type|SOCK_CLOEXEC, 0);
  if (sock != -1 || errno != EINVAL) return sock;
  // An "errno" of EINVAL likely means that the system wasn't happy with the SOCK_CLOEXEC; fall through and try again without it:
#endif

  sock = socket(AF_INET, type, 0);

#ifdef FD_CLOEXEC
  if (sock != -1) fcntl(sock, F_SETFD, FD_CLOEXEC);
#endif
  return sock;
}


int setupSocket(portNumBits port, Boolean makeNonBlocking){
	int newSocket = createSocket(SOCK_STREAM);
	if(newSocket < 0){
		LOG_E("unable to create stream socket\n");
		return newSocket;
	}

    //SO_REUSEADDR允许启动一个监听服务器并捆绑其众所周知的端口，即使以前建立的将该端口用作他们的本地端口的连接仍存在。
	int reuseFlag = 1;
	if(setsockopt(newSocket, SOL_SOCKET, SO_REUSEADDR,
		(const void *)&reuseFlag, sizeof(reuseFlag)) < 0){
		LOG_E("setsockopt(SO_REUSEADDR) error");
		closeSocket(newSocket);
		return -1;
	}

	struct sockaddr_in addr;
	makeSocketAddress(addr, ReceivingInterfaceAddr, port);
	if ( bind(newSocket, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		LOG_E("setsockopt(SO_REUSEADDR) error");
		closeSocket(newSocket);
		return -1;
	}

	if(makeNonBlocking){
		if(!makeSocketNonBlocking(newSocket)){
	  		LOG_E("failed to make non-blocking");
	  		closeSocket(newSocket);
      		return -1;
  		}
	}

	return newSocket;
}


int readSocket(int socket, unsigned char* buffer, unsigned bufferSize,
	       struct sockaddr_in& fromAddress)
{
	socklen_t addrSize = sizeof(fromAddress);
	int bytesRead = recvfrom(socket, (char *)buffer, bufferSize, 0, (struct  sockaddr *)&fromAddress, &addrSize);
	if(bytesRead < 0){
		int err = errno;
		if (err == 111 /*ECONNREFUSED (Linux)*/
		 || err == EAGAIN
		 || err == 113 /*EHOSTUNREACH (Linux)*/) { 
		 // Why does Linux return this for datagram sock?
	      	fromAddress.sin_addr.s_addr = 0;
	      	return 0;
     	}
	} else if ( bytesRead == 0){
      // "recvfrom()" on a stream socket can return 0 if the remote end has closed the connection.  Treat this as an error:
		return -1;
	}

	return bytesRead;
}

Boolean writeSocket(int socket, struct in_addr address, 
			portNumBits portNum, unsigned char ttlArg,
		    unsigned char* buffer, unsigned bufferSize) {
  // Before sending, set the socket's TTL:
  unsigned char ttl = (unsigned char)ttlArg;
  if (setsockopt(socket, IPPROTO_IP, IP_MULTICAST_TTL,
		 (const char*)&ttl, sizeof ttl) < 0) {
    perror("setsockopt(IP_MULTICAST_TTL) error: ");
    return false;
  }

  return writeSocket(socket, address, portNum, buffer, bufferSize);
}

Boolean writeSocket(int socket, struct in_addr address, portNumBits portNum,
		    unsigned char* buffer, unsigned bufferSize) {
  	do{
		struct sockaddr_in dest;
		dest.sin_family = AF_INET;
	    dest.sin_addr.s_addr = address.s_addr;
	    dest.sin_port = portNum;
	    //dest.sin_len = sizeof(dest);

	    int bytesSent = sendto(socket, (char*)buffer, bufferSize, 0,
				   (struct sockaddr*)&dest, sizeof dest);
	    if (bytesSent != (int)bufferSize) {
	      char tmpBuf[100];
	      sprintf(tmpBuf, "writeSocket(%d), sendTo() error: wrote %d bytes instead of %u: ", socket, bytesSent, bufferSize);
	      perror(tmpBuf);
	      break;
	  	}
	  	return true;
  	}while(0);

  	return false;
}