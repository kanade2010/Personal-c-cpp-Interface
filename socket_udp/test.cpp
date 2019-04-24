#include "UdpSocket.hpp"
#include <stdio.h>
#include <unistd.h>
//#include <string>
int main(int argc,char**argv)
{
  int iSend  = 0, iCreate = 0, iRevStat = 0;
  UdpSocket  Udp;
  iCreate = Udp.Create(8000);
  while(1){
  Udp.ReceiveForm();
  printf("%s, %d : %s\n",Udp.GetClientIp(), Udp.GetClientPort(), Udp.GetRevBuffer());
  iSend = Udp.SendTo("Hello, world", 13, "192.168.0.78", 10000);
  }
  //iRevStat = Udp.ReceiveForm();
  //puts(Udp.GetRevDate());
  //puts(Udp.GetClientIp());
 //iRevStat =  Udp.Broadcast("songwei",7777);
  //printf("Broadcast %d\n",iRevStat);

}