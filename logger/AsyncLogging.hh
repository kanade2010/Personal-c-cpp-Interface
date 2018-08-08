#ifndef _ASYNC_LOGGING_HH
#define _ASYNC_LOGGING_HH
#include <MutexLock.hh>
#include "Thread.hh"
#include <string>

class AsyncLogging
{
public:
	AsyncLogging(const std::string filePath, off_t rollSize, int flushInterval = 3);
	~AsyncLogging();

	void append(const char *logline, int len);

private:
	AsyncLogging(const AsyncLogging&);
	AsyncLogging& operator=(const AsyncLogging&);

	void threadRoutine();

	const int m_flushInterval;
	bool m_runing;
	off_t m_rollSize;
	std::string m_filePath;
	Thread m_thread;

	MutexLock m_mutex;

};

#endif
