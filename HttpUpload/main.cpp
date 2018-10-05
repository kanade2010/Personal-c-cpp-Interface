#include "InetAddress.hh"
#include "TcpClient.hh"
#include "HttpClient.hh"
#include "Logger.hh"
#include "SocketHelp.hh"
#include "tools.h"
#include "SyncLogging.hh"
#include "JsonStrParse.hh"

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fstream>
#include <map>
#include <time.h>

#define INTTOSTRING(SIZE) ( static_cast<std::stringstream&>(std::stringstream() << SIZE).str() )
const int kBufferSize = 4096;	//Socket Buffer Size

const std::string kBoundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";

//char gSn[13] = {0};
std::string gSn = "976890001624";
//std::string gSn = "978290000089";

//SyncLog 
const off_t kRollSize = 1024*200;

SyncLogging* g_synclog = NULL;

void SyncLogOutput(const char* logline, int len){
	g_synclog->append(logline, len);
}

void SyncLogFlush(){
	g_synclog->flush();
}

#define CONFFILEPATH "/home/gtbox/etc/tbox-param.conf"  // server parameter configure file 

void ReadConfInfo()
{
	unsigned char buf[100] = {0x0};

	LOG_TRACE << "ReadConfInfo() read sn";

	int key = 0xF002;
	memset(buf, 0, sizeof(buf));
	int ret = tools_read_bin_config(buf, &key, sizeof(unsigned short), CONFFILEPATH);
	assert(ret > 0);
	  gSn = std::string(reinterpret_cast<char*>(buf));

	LOG_INFO << "Sn : " << gSn;
}

static off_t FileSize(std::string path) // get file size
{
     struct stat fileInfo;
     if (stat(path.c_str(), &fileInfo) < 0)
     {
     	 switch(errno)
     	 {
     	 	case ENOENT:
     	 	  LOG_FATAL << "file : " << path << " inexistence";
     	 	default:
         	  LOG_FATAL << (stderr, "stat fileInfo failed : %s\n", strerror(errno));
         }
     }

     return fileInfo.st_size;
}

int main(int argc, char *argv[])
{

	/*SyncLogging log("/home/gtbox/log/http-upload.log", kRollSize);
	g_synclog = &log;
	Logger::setOutput(SyncLogOutput);   //redirect log to /home/gtbox/log/httpdown.log
	Logger::setFlush(SyncLogFlush);

	ReadConfInfo();*/

	if(argc != 2)
	{
		LOG_FATAL << "input parameter number error";
	}

	if(access(argv[1], 0) != 0)
	{
		LOG_FATAL << "input file inexits";
	}

//get token

	std::string uploadFile(argv[1]);
	LOG_DEBUG << "start upload file : " << uploadFile << "... ";

	HttpClient http("https://aep.machinestalk.com/api/Launch/GetVehicleToken/" + gSn + "/" + std::to_string(time(NULL)));

	http.setRequestMethod("GET");
	http.setRequestProperty("Content-type", "application/json");
	http.setRequestProperty("Cache-Control", "no-cache");
	http.setRequestProperty("Connection", "close\r\n");

	http.connect();
	http.send();
	http.handleRead();

// uoload

	if(http.getResponseCode() != 200)
	{
		LOG_ERROR << "http.getResponseCode() Error Response Code is : " << http.getResponseCode();
		return -1;
	}

	LOG_DEBUG << "Json Content : " << http.getResponseContent();

	std::string vehicleToken;
	vehicleToken = JsonParse::getVehicleToken(std::string(http.getResponseContent()));
	if(vehicleToken.empty())
	{
		LOG_ERROR << "getVehicleToken() error";
		return -1;
	}

	LOG_DEBUG << "vehicleToken : " << vehicleToken;

	int timeVal = time(NULL) + 10;
	std::string cmd = "echo -n \'" + gSn + vehicleToken + std::to_string(timeVal) + "\'| md5sum | cut -d \' \' -f1";
	LOG_DEBUG << "cmd : " << cmd; 

	FILE* fp = popen(cmd.c_str(), "r");
	char tmp[48] = {0};
	if(fgets(tmp, sizeof tmp, fp) == NULL) {
		LOG_ERROR << "fgets Sign Error";
		return -1;
	}
	pclose(fp);

	std::string sign(tmp);
		LOG_DEBUG << "Sign Size : " << sign.size();
	sign.erase(sign.end() - 1);
		LOG_DEBUG << "Sign Size : " << sign.size();
	LOG_DEBUG << "Sign : " << sign;

	std::string diagLogUrl = "https://aep.machinestalk.com/api/Launch/DiagLog/" + gSn + "/" + vehicleToken + "/" + sign + "/" + std::to_string(time(NULL));
	//std::string diagLogUrl = "https://aep.machinestalk.com/api/Launch/DiagLog/976890001624/YgQrWUPljZMbZSlysdKt0g/aa73d703d1ceb0b62aa28c7ffa6b80f0/1538275409";
	//for(int i= 0; i < diagLogUrl.size(); i++) printf("0x%02x%c", diagLogUrl[i], i != diagLogUrl.size() - 1 ?' ':'\n');

	off_t fileSize = FileSize(uploadFile);
	LOG_DEBUG << "fileSize : " << fileSize;
	std::stringstream content;
	content << "--" + kBoundary << "\r\n";
	content << "Content-Disposition: form-data; name=\"upload\"; filename=\"DiagnoseResultJson.json\"\r\n";
	content << "Content-Type: text/plain\r\n\r\n";

	std::string contentEnd = "\r\n--" + kBoundary + "--\r\n";

	HttpClient upload(diagLogUrl);
	upload.setRequestMethod("POST");
	upload.setRequestProperty("Content-Type", "multipart/form-data; boundary=" + kBoundary);
	upload.setRequestProperty("Cache-Control", "no-cache");
	upload.setRequestProperty("Content-Length", std::to_string(fileSize + content.str().size() + contentEnd.size()));
	upload.setRequestProperty("Connection", "close\r\n");
	upload.setRequestBody(content.str());

	std::cout << upload.strStream() << std::endl;

	upload.connect();
	upload.send();
	upload.uploadFile(uploadFile, contentEnd);
	upload.handleRead();

	return 0;
}
