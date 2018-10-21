#include "HttpRequest.hh"
#include "Logger.hh"

int main()
{
  /*
  HttpRequest ImageReq("https://dlc2.pconline.com.cn/filedown_870344_9474088/l0WE76og/apploc.msi");
  ImageReq.setRequestMethod("GET");
  ImageReq.setRequestProperty("Cache-Control", "no-cache");
  ImageReq.setRequestProperty("Content-Type", "application/octet-stream");
  ImageReq.setRequestProperty("Connection", "Keep-Alive\r\n");

  ImageReq.connect();
  ImageReq.send();
  ImageReq.handleRead();
  ImageReq.downloadFile("./test.file");
*/


    HttpRequest appDownReq("https://cngldlcenter.x431.com/pointdown?versionDetailId=22807");// + versionDetailId);
    appDownReq.setRequestMethod("GET");
    appDownReq.setRequestProperty("Cache-Control", "no-cache");
    appDownReq.setRequestProperty("Content-Type", "application/octet-stream");
    appDownReq.setRequestProperty("Connection", "Keep-Alive\r\n");

    LOG_INFO << "Send Get Request : \n" << appDownReq.strStream();

    appDownReq.connect();
    appDownReq.send();
    appDownReq.handleRead();
    appDownReq.downloadFile("./install.tar.gz");
    LOG_INFO << "DownLoad App Update File Successfully\n";

  return 0;

}