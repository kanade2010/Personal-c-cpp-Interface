#include "InetAddress.hh"
#include "TcpClient.hh"
#include "HttpClient.hh"
#include "Logger.hh"
#include "SocketHelp.hh"
#include "Markup.hh"
#include "tools.h"
#include "SyncLogging.hh"

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fstream>
#include <map>
#include <time.h>

#define INTTOSTRING(SIZE) ( static_cast<std::stringstream&>(std::stringstream() << SIZE).str() )
const int kBufferSize = 4096;	//Socket Buffer Size

//char gSn[13] = {0};

extern std::string gServerMcuVersion;

std::string gMcuVersion = "00.00";
std::string gAppVersion = "0.00.000";

std::string gAppType;
std::string gMcuType;
std::string gUrl;
std::string gAreaType = "EN";

//char gSn[13] = {0};
std::string gSn = "977880000068";
//std::string gSn = "978290000089";

//SyncLog 
const off_t kRollSize = 1024*200;

SyncLogging* g_synclog = NULL;

void SyncLogOutput(const char* logline, int len){
	g_synclog->append(logline, len);
}

const std::string kAppUpdatePath = "/usrdata/update/app/";
std::string kMcuUpdatePath = "/usrdata/update/";
std::string kMcuUpdateFile;
const std::string kAppDownPath   = kAppUpdatePath + "Install.tar.bz2";
const std::string kMcuDownPath   = "/usrdata/download.zip";
const std::string kAppStatFlag   = kAppUpdatePath + "upgrade_app_flag";
const std::string kMcuStatFlag   = "/usrdata/upgrade_mcu_flag";

bool CheckXml(CMarkup &xml, bool &isNeedUpgreade, std::string &downloadAddrId)
{

	bool ret = false;
	//LOG_DEBUG << xml.GetDoc() << std::endl;
	if(xml.FindChildElem("soap:Body"))
	{
		LOG_DEBUG << "CheckXml() : Find Elem soap:Body";
		xml.IntoElem();
		if(xml.FindChildElem("ns2:getPublicSoftMaxVersionByNameResponse"))
		{
			LOG_DEBUG << "CheckXml() : Find Elem ns2:getPublicSoftMaxVersionByNameResponse";
			xml.IntoElem();
			if(xml.FindChildElem("return"))
			{
				LOG_DEBUG << "CheckXml() : Find Elem return";
				xml.IntoElem();
				if(xml.FindChildElem("code"))
				{	
					LOG_DEBUG << "CheckXml() : Find Elem code";
					std::string code;
					if( (code = xml.GetChildData()) != "0"){
						if(code == "405") LOG_ERROR << "Server : This device is not supported"; 
						else
						LOG_ERROR << "CheckXml() : check code error, code is " << code;
						return false;
					}
					if(xml.FindChildElem("SoftMaxVersion"))
					{
						LOG_DEBUG << "CheckXml() : Find Elem SoftMaxVersion";
						xml.IntoElem();
						if(xml.FindChildElem("forceUpgrade"))
						{
							if(xml.GetChildData() == "1")
								isNeedUpgreade = true;
							else 
								isNeedUpgreade = false;
							
							LOG_DEBUG << "CheckXml() : forceUpgrade : " << isNeedUpgreade;

							if(xml.FindChildElem("versionDetailId"))
							{
								downloadAddrId = xml.GetChildData();
								LOG_DEBUG << "CheckXml() : versionDetailId : " << downloadAddrId.c_str();
								ret = true;
							}
						}
						if(xml.FindChildElem("versionNo"))
								LOG_DEBUG << "CheckXml() : versionNo : " << xml.GetChildData().c_str();
					}	
				}
			}
		}
	}

	return ret;
}

void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
  std::string::size_type pos1, pos2;
  pos2 = s.find(c);
  pos1 = 0;
  while(std::string::npos != pos2)
  {
    v.push_back(s.substr(pos1, pos2-pos1));
 
    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }
  if(pos1 != s.length())
    v.push_back(s.substr(pos1));
}

bool IsNeedUpgreade(std::string &downloadAddrId)
{
	char buffer[kBufferSize] = {0};
	size_t nread = 0;
	size_t writtenBytes = 0;
	bool isEnd = false;

	while(!isEnd)
	{
		nread = sockets::read(g_sockfd, buffer, kBufferSize);
		size_t remain = kBufferSize - nread;
		while(remain > 0)
		{
			size_t n = sockets::read(g_sockfd, buffer + nread, remain);
			if(n < 0) LOG_SYSFATAL << "sockets::read";
			if(0 == n)
			{

				LOG_DEBUG << "sockets::read finish";
				isEnd = true;
				break;
			}
			remain = remain - n;
			nread += n;
		}
	}

	LOG_DEBUG << "Http Response :\n" << buffer;

	std::string line;
	std::stringstream ss(buffer);
	std::vector<std::string> v;
	getline(ss, line);
	SplitString(line, v, " ");
	//for(int i = 0; i < v.size(); i++) std::cout << v[i] << std::endl;
	if(v[1] != "200")
	{
	  LOG_ERROR << "Check version Failed, Server Response : " << v[1].c_str();
	  return false; 
	} 

	CMarkup xml;
	xml.SetDoc(buffer);
	bool isNeedUpgreade = false;
	CheckXml(xml, isNeedUpgreade, downloadAddrId);

	LOG_DEBUG << isNeedUpgreade << " : " << downloadAddrId.c_str();
	
	sockets::close(g_sockfd);

	return isNeedUpgreade;
}


int main(int argc, char *argv[])
{
	/*SyncLogging log("http-update.log", kRollSize);
	g_synclog = &log;
	Logger::setOutput(SyncLogOutput);   //redirect log to /home/gtbox/log/http-update.log

	LOG_TRACE << "Get Device version Info";
	*/

//App Version Req
	HttpClient appVerReq("https://usglmycar.x431.com/services/publicSoftService");
	//std::string content = "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:x431=\"http://www.x431.com\"><soapenv:Header/><soapenv:Body><x431:getPublicSoftMaxVersionByName><serialNo>978290000089</serialNo><publicSoftName>Gtbox_Mini_main_app_DB</publicSoftName><versionNo>1.00.002</versionNo><displayLan>CN</displayLan></x431:getPublicSoftMaxVersionByName></soapenv:Body></soapenv:Envelope>";
	std::string content = "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org\\
/soap/envelope/\" xmlns:x431=\"http://www.x431.com\"><soapenv:Header/><soapenv:Body>\\
<x431:getPublicSoftMaxVersionByName><serialNo>" + gSn + "</serialNo><pub\\
licSoftName>main_apk_DB</publicSoftName><versionNo>" + gAppVersion\
 + "</versionNo><displayLan>" + gAreaType + "</displayLan></x431:getPublicSoftMaxVersionByName></soapenv:\\
Body></soapenv:Envelope>";

	appVerReq.setRequestMethod("POST");
	appVerReq.setRequestProperty("Content-type", "text/xml;charset=UTF-8");
	appVerReq.setRequestProperty("Cache-Control", "no-cache");
	appVerReq.setRequestProperty("Connection", "close");
	appVerReq.setRequestProperty("Content-Length", INTTOSTRING(content.size()) + "\r\n");
	appVerReq.setRequestBody(content);

	LOG_INFO << "Send POST Request : \n" << appVerReq.strStream();

	appVerReq.connect();
	appVerReq.send();

	std::string versionDetailId;
	if(!IsNeedUpgreade(versionDetailId))
	{
      LOG_INFO << "no need to update " << "App.";
	}
	else
	{
//download app update file
		HttpClient appDownReq("https://cngldlcenter.x431.com/pointdown?versionDetailId=" + versionDetailId);
		appDownReq.setRequestMethod("GET");
		appDownReq.setRequestProperty("Cache-Control", "no-cache");
		appDownReq.setRequestProperty("Content-Type", "application/octet-stream");
		appDownReq.setRequestProperty("Connection", "close\r\n");

		LOG_INFO << "Send Get Request : \n" << appDownReq.strStream();

		appDownReq.connect();
		appDownReq.send();
		appDownReq.handleRead();
		appDownReq.downloadFile("./install.tar.gz");
		LOG_INFO << "DownLoad App Update File Successfully\n";
	}
//check and down mcu
	HttpClient http("https://cnpublicdlcenter.x431.com/public/goloSoftUpgrade.action?serialNo=" + gSn + "&clientVersionNo=" + gMcuVersion);

	http.setRequestMethod("GET");
	//http.setRequestProperty("Content-type", "application/json");
	http.setRequestProperty("Cache-Control", "no-cache");
	http.setRequestProperty("Connection", "close\r\n");

	http.connect();
	http.send();
	http.handleRead();

	if(gServerMcuVersion.empty())
	{
		LOG_ERROR << "No need to update Mcu Get Msg : " << http.getResponseContent();
		return -1;
	}

	gMcuVersion.erase(0 , gMcuVersion.find_first_not_of('0'));
	gServerMcuVersion.erase(0 , gServerMcuVersion.find_first_not_of('0'));
	LOG_DEBUG << "Server Mcu Version : " << gServerMcuVersion << " | Device Mcu Version :" << gMcuVersion;

	if(gServerMcuVersion <= gMcuVersion)
	{
		LOG_DEBUG << "No need to update Mcu  Exit.";
		return -1;
	}

	http.downloadFile("Download.bin");
	LOG_INFO << "DownLoad Mcu Update File Successfully\n";

	return 0;
}
