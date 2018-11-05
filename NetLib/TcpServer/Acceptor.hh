#ifndef _ACCEPTOR_HH
#define _ACCEPTOR_HH
#include <functional>

#include "Channel.hh"
#include "Socket.hh"

class EventLoop;
class InetAddress;

class Acceptor{
public:
  typedef std::function<void (int sockfd, const InetAddress&)> NetConnectionCallBack;

  Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport);

  bool listenning() const;
  void listen();

  void setNewConnectionCallBack(cont NetConnectionCallBack& cb);

private:
  Acceptor& operator=(const Acceptor&);
  Acceptor(const Acceptor&);

  void handleRead();

  EventLoop* p_loop;
  Socket m_acceptSocket;
  Channel m_acceptChannel;
  NewConnectionCallBack m_newConnectionCallBack;
  bool m_listenning;
  int m_idleFd;
};


#endif