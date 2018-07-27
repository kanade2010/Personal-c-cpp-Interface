#ifndef _NET_INTERFACE_HH
#define _NET_INTERFACE_HH

class NetInterface {
public:
  virtual ~NetInterface();

protected:
  NetInterface(); // virtual base class
};

#endif