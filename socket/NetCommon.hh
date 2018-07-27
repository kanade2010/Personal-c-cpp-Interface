#ifndef _NET_COMMON_HH
#define _NET_COMMON_HH

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

typedef int Port;
typedef bool Boolean;
typedef unsigned int IpAddrBits;
typedef uint16_t portNumBits;
typedef unsigned int netAddressBits;

#define closeSocket close

#endif