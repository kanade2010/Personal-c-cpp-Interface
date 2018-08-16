#ifndef _ASYNC_LOGGING_HH
#define _ASYNC_LOGGING_HH
#include "MutexLock.hh"
#include "LogStream.hh"
#include "LogFile.hh"
#include "Logger.hh"
#include <string>

class BlockingLog
{
public:
	//BlockingLog(const std::string filePath, off_t rollSize, int flushInterval = 0);
	BlockingLog(const std::string platformLogPath, const std::string m_comLogPath, const std::string m_runLogPath, off_t rollSize);
	~BlockingLog();

	//void append(const char *logline, int len);
	void append(const char *logline, int len, Logger::LogType logtype);

private:
	BlockingLog(const BlockingLog&);
	BlockingLog& operator=(const BlockingLog&);

	typedef LogBuffer<kLargeBuffer> Buffer;
	typedef scoped_ptr<Buffer> BufferPtr;

	Buffer* getBufferPtrFromType(Logger::LogType logtype);

	const int m_flushInterval;
	off_t m_rollSize;
	//std::string m_filePath;
	MutexLock m_mutex;

	//BufferPtr m_pCurrentBuffer;
	//Buffer m_Buffers;
	//LogFile m_logFile;

	std::string m_platformLogPath;
	std::string m_comLogPath;
	std::string m_runLogPath;

	LogFile m_platformLogFile;
	LogFile m_comLogFile;
	LogFile m_runLogFile;

	Buffer m_platformBuffers;
	Buffer m_comBuffers;
	Buffer m_runBuffers;

	BufferPtr m_pplatformBuffer;
	BufferPtr m_pcomBuffer;
	BufferPtr m_prunBuffer;
};

#endif
