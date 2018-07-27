#ifndef _SOCKET_HELP_HH
#define _SOCKET_HELP_HH

#ifndef _NET_COMMON_HH
#include "NetCommon.hh"
#endif

static struct sockaddr_in dummy; //from address not used, as default parameters.

void delaySecond(int sec);
Boolean makeSocketNonBlocking(int sock);

int setupSocket(PortNumBits port, Boolean makeNonBlocking = true);

int readSocket(int socket, unsigned char* buffer, unsigned bufferSize,
	       struct sockaddr_in& fromAddress = dummy);
Boolean writeSocket(int socket, struct in_addr address, 
			PortNumBits portNum, unsigned char ttlArg,
		    unsigned char* buffer, unsigned bufferSize);
Boolean writeSocket(int socket, struct in_addr address, PortNumBits portNum,
		    unsigned char* buffer, unsigned bufferSize);
#endif