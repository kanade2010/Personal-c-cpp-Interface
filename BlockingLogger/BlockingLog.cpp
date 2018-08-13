#include "BlockingLog.hh"
#include "LogFile.hh"
#include <assert.h>
#include <stdio.h>

BlockingLog::BlockingLog(const std::string filePath, off_t rollSize, int flushInterval)
	:m_filePath(filePath),
	 m_rollSize(rollSize),
	 m_flushInterval(flushInterval),
	 m_mutex(),
	 m_pCurrentBuffer(new Buffer),
	 m_logFile(m_filePath, m_rollSize, false, m_flushInterval)
{
}

BlockingLog::~BlockingLog(){
	m_logFile.append(m_pCurrentBuffer->data(), m_pCurrentBuffer->length());
	m_logFile.flush();
}

void BlockingLog::append(const char* logline, int len){
	MutexLockGuard lock(m_mutex);
	if(m_pCurrentBuffer->avail() > len){
		m_pCurrentBuffer->append(logline, len);
	}
	else{
		m_logFile.append(m_pCurrentBuffer->data(), m_pCurrentBuffer->length());
		
		m_pCurrentBuffer->reset();

		if(m_pCurrentBuffer->avail() > len){
			m_pCurrentBuffer->append(logline, len);
		}
	}
}
