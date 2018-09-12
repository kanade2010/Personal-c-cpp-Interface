#include "InetAddress.hh"
#include "SocketHelp.hh"
#include "assert.h"
#include <strings.h>

InetAddress::InetAddress(StringArg ip, uint16_t port)
{

    ::bzero(&m_addr, sizeof m_addr);
    sockets::fromIpPort(ip.c_str(), port, &m_addr);
}

/*string InetAddress::toIpPort() const
{
  char buf[64] = "";
  sockets::toIpPort(buf, sizeof buf, getSockAddr());
  return buf;
}

string InetAddress::toIp() const
{
  char buf[64] = "";
  sockets::toIp(buf, sizeof buf, getSockAddr());
  return buf;
}*/

uint32_t InetAddress::ipNetEndian() const
{
  assert(family() == AF_INET);
  return m_addr.sin_addr.s_addr;
}
