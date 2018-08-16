#include "BlockingLog.hh"
#include "LogFile.hh"
#include <assert.h>
#include <stdio.h>
/*
BlockingLog::BlockingLog(const std::string filePath, off_t rollSize, int flushInterval)
	:m_filePath(filePath),
	 m_rollSize(rollSize),
	 m_flushInterval(flushInterval),
	 m_mutex(),
	 m_pCurrentBuffer(new Buffer),
	 m_logFile(m_filePath, m_rollSize, false, m_flushInterval)
{
}*/

BlockingLog::BlockingLog(const std::string platformLogPath, const std::string comLogPath, const std::string runLogPath, off_t rollSize)
	: m_platformLogPath(platformLogPath),
	  m_comLogPath(comLogPath),
	  m_runLogPath(runLogPath),
	  m_pplatformBuffer(new Buffer),
	  m_pcomBuffer(new Buffer),
	  m_prunBuffer(new Buffer),
	  m_rollSize(rollSize),
	  m_flushInterval(0),
	  m_runLogFile(m_runLogPath, m_rollSize, false, 0),
	  m_platformLogFile(m_platformLogPath, m_rollSize, false, 0),
	  m_comLogFile(m_comLogPath, m_rollSize, false, 0){

}


BlockingLog::~BlockingLog(){
	m_platformLogFile.append(m_pplatformBuffer->data(), m_pplatformBuffer->length());
	m_platformLogFile.flush();
	m_comLogFile.append(m_pcomBuffer->data(), m_pcomBuffer->length());
	m_comLogFile.flush();
	m_runLogFile.append(m_prunBuffer->data(), m_prunBuffer->length());
	m_runLogFile.flush();
}
/*
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
}*/

void BlockingLog::append(const char *logline, int len, Logger::LogType logtype){
	MutexLockGuard lock(m_mutex);
	Buffer *p = getBufferPtrFromType(logtype);

	if(p->avail() > len){
		p->append(logline, len);
	}
	else{
		if(logtype == Logger::PLATFORM) 
			m_platformLogFile.append(p->data(), p->length());
		if(logtype == Logger::COM) 
			m_comLogFile.append(p->data(), p->length());
		if(logtype == Logger::RUN) 
			m_runLogFile.append(p->data(), p->length());
		
		p->reset();

		if(p->avail() > len){
			p->append(logline, len);
		}
	}
}


BlockingLog::Buffer* BlockingLog::getBufferPtrFromType(Logger::LogType logtype){
	if(logtype == Logger::PLATFORM) return m_pplatformBuffer.get();
	if(logtype == Logger::COM) return m_pcomBuffer.get();
	if(logtype == Logger::RUN) return m_prunBuffer.get();
}
