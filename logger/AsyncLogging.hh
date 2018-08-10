#ifndef _ASYNC_LOGGING_HH
#define _ASYNC_LOGGING_HH
#include "MutexLock.hh"
#include "Thread.hh"
#include "LogStream.hh"
#include "ptr_vector.hh"
#include "Condition.hh"

#include <string>

class AsyncLogging
{
public:
	AsyncLogging(const std::string filePath, off_t rollSize, int flushInterval = 3);
	~AsyncLogging();

	void start(){
		m_runing = true;
		m_thread.start();
	}

	void append(const char *logline, int len);

private:
	AsyncLogging(const AsyncLogging&);
	AsyncLogging& operator=(const AsyncLogging&);

	void threadRoutine();

	typedef LogBuffer<kLargeBuffer> Buffer;
	typedef oneself::ptr_vector<Buffer> BufferVector;
	typedef oneself::auto_ptr<Buffer> BufferPtr;

	const int m_flushInterval;
	bool m_runing;
	off_t m_rollSize;
	std::string m_filePath;
	Thread m_thread;
	MutexLock m_mutex;
	Condition m_cond;

	BufferPtr m_pCurrentBuffer;
	BufferVector m_vBuffers;
};

#endif
