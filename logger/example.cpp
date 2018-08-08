#include "ScopePtr.hh"
#include "Logger.hh"
#include "LogStream.hh"
#include "FileUtil.hh"
#include "LogFile.hh"
#include "AsyncLogging.hh"
#include <errno.h>

int main(){

/*
	//scoped_ptr test
	scoped_ptr<LogStream> lsp(new LogStream);
	
	LogStream()<<"stream test\n";
	(*lsp)<<"scope_test\n";
*/
	
	//fileutil test
/*	FileUtil::AppendFile testfile("./testfile");
	testfile.append("12345\n", 6);
	testfile.append("54321\n", 6);
*/

	//LogFile test
/*	LogFile output("tttt");
	output.append("56789\n", 6);
	output.append("98765\n", 6);
*/

//AsyncLogging  thread test

	//AsyncLogging alog("2131", 12345);

//Log stdout test

	short it1=1;
	unsigned it2=2;
	int it3 =-3;
	long  it4 = 4;
	unsigned long long it5 = 5; 
	double dtest = 2.0/3;
	bool bt1 = false;
	bool bt2 = true;
	char *pt1 = NULL;
	char *pt2 = (char *)"abcdefg";

	printf("(%d  %d %d)\n", Logger::TRACE, Logger::DEBUG, Logger::logLevel());
	Logger::setLogLevel(Logger::TRACE);
	printf("(%d  %d %d)\n", Logger::TRACE, Logger::DEBUG, Logger::logLevel());

	//for(int i = 0; i< 200 ; i++){
		LOG_DEBUG << "int test" << ' ' << it1 << it2 << it3 << it4 << it5;
		LOG_DEBUG << "double test"<< ' ' << 3.1415926 << ' ' << dtest;
		LOG_DEBUG << "bool test"<< ' ' << bt1 << ' '<< bt2;
		LOG_DEBUG << "ptr test"<< ' ' << pt1 << ' '<<pt2;
		LOG_DEBUG << "just test";
	//}

	LOG_INFO <<" just Test ";
	LOG_TRACE <<" just Test ";
	LOG_WARN <<" just Test ";
	LOG_DEBUG <<" just Test ";
	

	LOG_ERROR <<" just Test E";

	//abort()
	//LOG_FATAL <<" just Test F";

	errno = 11;	
	LOG_SYSERR << " just Test SE";

	//abort()
	//LOG_SYSFATAL << " just Test SF";

	getchar();

	return 0;
}