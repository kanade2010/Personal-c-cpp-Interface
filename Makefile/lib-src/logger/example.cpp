#include "scoped_ptr.hh"
#include "Logger.hh"
#include "LogStream.hh"
#include "FileUtil.hh"
#include "LogFile.hh"
#include "BlockingLog.hh"
#include "TimeStamp.hh"
#include <errno.h>
#include <unistd.h>

const off_t kRollSize = 2048*100;

BlockingLog* g_blockingLog = NULL;

void blockingOutputWithType(const char* logline, int len, Logger::LogType logtype){
	g_blockingLog->append(logline, len, logtype);
}

#define PLOG_WARN Logger(__FILE__, __LINE__, Logger::WARN, Logger::PLATFORM).stream()
#define CLOG_DEBUG Logger(__FILE__, __LINE__, Logger::DEBUG, Logger::COM).stream()
#define RLOG_TRACE Logger(__FILE__, __LINE__, Logger::TRACE, Logger::RUN).stream()


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

//BlockingLog  thread test

	//BlockingLog alog("2131", 12345);

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
/*
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
	LOG_DEBUG << "test to write in ./test.log";*/
	//BlockingLog Test
	BlockingLog log("./platform.log", "./com.log", "./run.log", kRollSize);
	g_blockingLog = &log;

	Logger::setOutputWithType(blockingOutputWithType);

	PLOG_WARN << "test outputWithType PLATFORM\n";
	CLOG_DEBUG << "test outputWithType COM\n";
	RLOG_TRACE << "test outputWithType RUN\n";


{

		TimeStamp m_time(TimeStamp::now());
		int64_t microSecondsSinceEpoch = m_time.microSecondsSinceEpoch();
		time_t seconds = static_cast<time_t>(microSecondsSinceEpoch / TimeStamp::kMicroSecondsPerSecond);
		int microseconds = static_cast<int>(microSecondsSinceEpoch % TimeStamp::kMicroSecondsPerSecond);
		struct tm tm_time;

		::gmtime_r(&seconds, &tm_time); // FIXME TimeZone::fromUtcTime

		int len = printf("%4d-%02d-%02d %02d:%02d:%02d\n",
		tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
		tm_time.tm_hour + 8, tm_time.tm_min, tm_time.tm_sec);
}
	for(int j = 0 ; j < 6 ; ++j){
	for (int i = 0; i < 20000; ++i){

		PLOG_WARN << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz ";
		CLOG_DEBUG << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz ";
		RLOG_TRACE << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz ";

	}
	printf("sleep 1\n");
	sleep(1);
}


{
		TimeStamp m_time(TimeStamp::now());
		int64_t microSecondsSinceEpoch = m_time.microSecondsSinceEpoch();
		time_t seconds = static_cast<time_t>(microSecondsSinceEpoch / TimeStamp::kMicroSecondsPerSecond);
		int microseconds = static_cast<int>(microSecondsSinceEpoch % TimeStamp::kMicroSecondsPerSecond);
		struct tm tm_time;

		::gmtime_r(&seconds, &tm_time); // FIXME TimeZone::fromUtcTime

		int len = printf("%4d-%02d-%02d %02d:%02d:%02d\n",
		tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
		tm_time.tm_hour + 8, tm_time.tm_min, tm_time.tm_sec);
}

	getchar();

	return 0;
}