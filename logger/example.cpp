#include "scoped_ptr.hh"
#include "Logger.hh"
#include "LogStream.hh"
#include "FileUtil.hh"
#include "LogFile.hh"
#include "AsyncLogging.hh"
#include "TimeStamp.hh"
#include <errno.h>

const off_t kRollSize = 2048*1000;

AsyncLogging* g_asynclog = NULL;

void asyncOutput(const char* logline, int len){
	g_asynclog->append(logline, len);
}



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
/*
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
*/

	//AsyncLogging Test
	AsyncLogging log("./test.log", kRollSize);
	log.start();
	g_asynclog = &log;

	Logger::setOutput(asyncOutput);

{

		TimeStamp m_time(TimeStamp::now());
		int64_t microSecondsSinceEpoch = m_time.microSecondsSinceEpoch();
		time_t seconds = static_cast<time_t>(microSecondsSinceEpoch / TimeStamp::kMicroSecondsPerSecond);
		int microseconds = static_cast<int>(microSecondsSinceEpoch % TimeStamp::kMicroSecondsPerSecond);
		struct tm tm_time;

		::gmtime_r(&seconds, &tm_time); // FIXME TimeZone::fromUtcTime

		int len = printf("%4d-%02d-%02d %02d:%02d:%02d",
		tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
		tm_time.tm_hour + 8, tm_time.tm_min, tm_time.tm_sec);
}

	for (int i = 0; i < 100; ++i){

		LOG_INFO << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz ";

	}


{
		TimeStamp m_time(TimeStamp::now());
		int64_t microSecondsSinceEpoch = m_time.microSecondsSinceEpoch();
		time_t seconds = static_cast<time_t>(microSecondsSinceEpoch / TimeStamp::kMicroSecondsPerSecond);
		int microseconds = static_cast<int>(microSecondsSinceEpoch % TimeStamp::kMicroSecondsPerSecond);
		struct tm tm_time;

		::gmtime_r(&seconds, &tm_time); // FIXME TimeZone::fromUtcTime

		int len = printf("%4d-%02d-%02d %02d:%02d:%02d",
		tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
		tm_time.tm_hour + 8, tm_time.tm_min, tm_time.tm_sec);
}

	getchar();

	return 0;
}