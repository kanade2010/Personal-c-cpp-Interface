#ifndef _SOCKET_HH
#define _SOCKET_HH

#ifndef _SOCKET_HELP_HH
#include "SocketHelp.hh"
#endif

#ifndef _NET_COMMON_HH
#include "NetCommon.hh"
#endif

#ifndef _NET_INTERFACE_HH
#include "NetInterface.hh"
#endif

class Socket{
public:
	virtual ~Socket();
	void reset();
	SocketNumBits socketNum();
	Socket();
	Socket(PortNumBits port);
private:
	SocketNumBits fSocketNum;
	PortNumBits fPortNum;
};

#endif