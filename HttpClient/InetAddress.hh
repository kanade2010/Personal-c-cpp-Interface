#ifndef _INET_ADDRESS_HH
#define _INET_ADDRESS_HH

#include <netinet/in.h>
#include "StringPiece.hh"

class InetAddress
{
 public:
  /// Constructs an endpoint with given ip and port.
  /// @c ip should be "1.2.3.4"
  InetAddress(StringArg ip, uint16_t port);

  /// Constructs an endpoint with given struct @c sockaddr_in
  /// Mostly used when accepting new connections
  explicit InetAddress(const struct sockaddr_in& addr)
    : m_addr(addr)
  { }

  sa_family_t family() const { return m_addr.sin_family; }
  //std::string toIp() const;
  //std::string toIpPort() const;

  const struct sockaddr* getSockAddr() const { return (struct sockaddr*)(&m_addr); }
  
  uint32_t ipNetEndian() const;

  // resolve hostname to IP address, not changing port or sin_family
  // return true on success.
  // thread safe
  // static bool resolve(StringArg hostname, StringArg* ip);
  // static std::vector<InetAddress> resolveAll(const char* hostname, uint16_t port = 0);

 private:
    struct sockaddr_in m_addr;

};

#endif