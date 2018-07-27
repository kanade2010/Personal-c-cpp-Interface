/*
 *
 * 
 */

#ifndef _RTSP_SERVER_HH
#define _RTSP_SERVER_HH

#ifndef _NET_COMMON_HH
#include "NetCommon.hh"


class RTSPServer{
public:
  static RTSPServer* createNew(Port ourPot, unsigned reclamationTestSeconds = 65);

protected:
  RTSPServer(int ourSocket, Port ourPort, unsigned reclamationTestSeconds);
  // called only by createNew();
  virtual ~RTSPServer();
  
};

#endif