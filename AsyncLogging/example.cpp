#include "scoped_ptr.hh"
#include "Logger.hh"
#include "LogStream.hh"
#include "FileUtil.hh"
#include "LogFile.hh"
#include "AsyncLogging.hh"
#include "TimeStamp.hh"
#include "auto_ptr.hh"
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <memory>  
#include <mutex>

const off_t kRollSize = 4096*1000*10;

AsyncLogging* g_asynclog = NULL;

std::mutex g_mutex;
int g_wait=0;

void asyncOutput(const char* logline, int len){
	g_asynclog->append(logline, len);
}

void AsyncFlush()
{
	g_asynclog->stop();
}

class test{
public:
	test(){printf("construct\n" );}
	~test(){printf("~deconstruct\n");}
	void print(){printf("%d\n", m);}
private:
	int m;
};

void test_write()
{
	for (int i = 0; i < 100000; ++i){
		//std::cout << i << ": "<< "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz ";
		LOG_INFO << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz ";

	}
	std::lock_guard<std::mutex> lock(g_mutex);
	g_wait++;
}


void ptintTime()
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

		Fmt us(".%06d ", microseconds);
		assert(us.length() == 8);
		printf(":%s\n", us.data());

}

int main(){
{
	AsyncLogging log("./test.log", kRollSize);
	log.start();
	g_asynclog = &log;

	Logger::setOutput(asyncOutput);
	Logger::setFlush(AsyncFlush);

	ptintTime();
	sleep(1);
	for(int j = 0 ; j < 1 ; ++j){
		for (int i = 1; i <= 100; ++i){
			LOG_WARN << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz ";
			LOG_DEBUG << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz ";
			LOG_TRACE << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz ";
			if(i == 90) 
			{	
				LOG_SYSFATAL << "FATAL ： test reclaim";
			}
		}
	//printf("sleep 1\n");
	//sleep(1);
	}

	ptintTime();
}


	/*	Thread write_thread1(test_write);
		Thread write_thread2(test_write);
		Thread write_thread3(test_write);
		Thread write_thread4(test_write);
	
	ptintTime();
	
		write_thread1.start();
		write_thread2.start();
		write_thread3.start();
		write_thread4.start();

		while(g_wait < 4) ;
	
	ptintTime();*/

	getchar();

	return 0;
}