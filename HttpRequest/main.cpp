#include "HttpRequest.hh"
#include "Logger.hh"

int main()
{

  HttpRequest ImageReq("http://img.zcool.cn/community/01ddc256eb71586ac7257d209712b7.jpg@1280w_1l_2o_100sh.jpg");
  ImageReq.setRequestMethod("GET");
  ImageReq.setRequestProperty("Cache-Control", "no-cache");
  ImageReq.setRequestProperty("Content-Type", "application/octet-stream");
  ImageReq.setRequestProperty("Connection", "close\r\n");

  ImageReq.connect();
  ImageReq.send();
  ImageReq.handleRead();
  ImageReq.downloadFile("./test.jpg");

  return 0;

}
