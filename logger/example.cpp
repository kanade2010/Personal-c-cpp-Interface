
#include "Logger.hh"
#include "LogStream.hh"

int main(){
	LogStream re;
	int a = 5;
	bool b = true;
	float c = 3.141592654;
	void *vop = &c;
	//re<< a << 'a' << b << c << vop << "fyou";


	Logger::LogLevel level;
	//re<< Logger::DEBUG << Logger::FATAL<<"\n";

	for(int i = 0; i< 200 ; i++){
		LOG_WARN << "just test";
		LOG_WARN << "just test";
		LOG_WARN << "just test";
		LOG_WARN << "just test";
		LOG_WARN << "just test";
	}
	return 0;
}