#ifndef _ASYNC_LOGGING_HH
#define _ASYNC_LOGGING_HH
#include "MutexLock.hh"
#include "LogStream.hh"
#include "LogFile.hh"
#include <string>

class BlockingLog
{
public:
	BlockingLog(const std::string filePath, off_t rollSize, int flushInterval = 3);
	~BlockingLog();

	void append(const char *logline, int len);

private:
	BlockingLog(const BlockingLog&);
	BlockingLog& operator=(const BlockingLog&);

	typedef LogBuffer<kLargeBuffer> Buffer;
	typedef scoped_ptr<Buffer> BufferPtr;

	const int m_flushInterval;
	bool m_runing;
	off_t m_rollSize;
	std::string m_filePath;
	MutexLock m_mutex;

	BufferPtr m_pCurrentBuffer;
	Buffer m_Buffers;
	LogFile m_logFile;
};

#endif
