#include "AsyncLogging.hh"
#include <stdio.h>

AsyncLogging::AsyncLogging(const std::string filePath, off_t rollSize, int flushInterval)
	:m_filePath("aa"),
	 m_rollSize(2048),
	 m_flushInterval(flushInterval),
	 m_thread(std::bind(&AsyncLogging::threadRoutine, this)),
	 m_mutex()
{

}

AsyncLogging::~AsyncLogging(){
}

void AsyncLogging::append(const char* logline, int len){
	MutexLockGuard lock(m_mutex);
}

void AsyncLogging::threadRoutine(){
	printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
	int i = 0xfffffff;
	while(i--);
}
