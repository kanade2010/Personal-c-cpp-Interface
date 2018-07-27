#include "Socket.hh"
#include "debug.h"

#define TAG "Socket"

Socket::Socket(){
	  LOG_D("wb+++1:Socket\n");
}

Socket::Socket(PortNumBits port):fPortNum(port),fSocketNum(setupSocket(port)){
  LOG_D("wb+++1:(%d)\n", fSocketNum);
}

void Socket::reset() {
  if (fSocketNum >= 0){
 	LOG_D("wb+++2: reset\n");
  	closeSocket(fSocketNum);
  }
  fSocketNum = -1;
}

Socket::~Socket(){
  reset();
  LOG_D("wb+++2: ~Socket\n");
}

SocketNumBits Socket::socketNum(){
	return fSocketNum;
}