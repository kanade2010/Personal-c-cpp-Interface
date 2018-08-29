#include "AsyncLogging.hh"
#include "LogFile.hh"
#include <assert.h>
#include <stdio.h>

AsyncLogging::AsyncLogging(const std::string filePath, off_t rollSize, int flushInterval)
	:m_filePath(filePath),
	 m_rollSize(2048),
	 m_flushInterval(flushInterval),
	 m_isRunning(false),
	 m_thread(std::bind(&AsyncLogging::threadRoutine, this)),
	 m_mutex(),
	 m_cond(m_mutex),
	 m_currentBuffer(new Buffer),
	 m_buffers()
{
}

AsyncLogging::~AsyncLogging(){
	if(m_isRunning) stop();
}

void AsyncLogging::append(const char* logline, int len){
	MutexLockGuard lock(m_mutex);
	if(m_currentBuffer->avail() > len){
		m_currentBuffer->append(logline, len);
	}
	else{
		m_buffers.push_back(m_currentBuffer.release());
		
		m_currentBuffer.reset(new Buffer);

		m_currentBuffer->append(logline, len);
		m_cond.notify();
	}
}

void AsyncLogging::threadRoutine(){
	assert(m_isRunning == true);
	LogFile output(m_filePath, m_rollSize, false);
	BufferVector buffersToWrite;
	buffersToWrite.reserve(8);

	while(m_isRunning){
		assert(buffersToWrite.empty());
		{
			MutexLockGuard lock(m_mutex);
			if(m_buffers.empty()){
				m_cond.waitForSeconds(m_flushInterval);
			}
			m_buffers.push_back(m_currentBuffer.release());
			m_currentBuffer.reset(new Buffer);
			m_buffers.swap(buffersToWrite);
		}

		assert(!buffersToWrite.empty());

		for(size_t i = 0; i < buffersToWrite.size(); ++i){
			output.append(buffersToWrite[i]->data(), buffersToWrite[i]->length());
		}

		buffersToWrite.clear();
		output.flush();
	}

	output.flush();
}
