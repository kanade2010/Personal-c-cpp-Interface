#include "AsyncLogging.hh"
#include "LogFile.hh"
#include <assert.h>
#include <stdio.h>

AsyncLogging::AsyncLogging(const std::string filePath, off_t rollSize, int flushInterval)
	:m_filePath("test.log"),
	 m_rollSize(2048),
	 m_flushInterval(flushInterval),
	 m_runing(false),
	 m_thread(std::bind(&AsyncLogging::threadRoutine, this)),
	 m_mutex(),
	 m_cond(m_mutex),
	 m_pCurrentBuffer(new Buffer),
	 m_vBuffers()
{
}

AsyncLogging::~AsyncLogging(){
}

void AsyncLogging::append(const char* logline, int len){
	MutexLockGuard lock(m_mutex);
	if(m_pCurrentBuffer->avail() > len){
		m_pCurrentBuffer->append(logline, len);
	}
	else{
		m_vBuffers.push_back(m_pCurrentBuffer.release());
	}
}

void AsyncLogging::threadRoutine(){
	assert(m_runing == true);
	LogFile output("./test.log", m_rollSize, false);
	BufferVector buffersToWrite;
	buffersToWrite.reserve(8);

	while(m_runing){
		{
			MutexLockGuard lock(m_mutex);
			if(!m_pCurrentBuffer.get()){
				BufferPtr tmp(new Buffer);
				m_pCurrentBuffer = tmp;
			}
			if(m_vBuffers.empty()){
				m_cond.waitForSeconds(m_flushInterval);
			}
			m_vBuffers.push_back(m_pCurrentBuffer.release());
			m_vBuffers.swap(buffersToWrite);
		}

		assert(!buffersToWrite.empty());

		for(size_t i = 0; i < buffersToWrite.size(); ++i){
			output.append(buffersToWrite[i]->data(), buffersToWrite[i]->length());
		}

		buffersToWrite.clear();
		output.flush();
	}

}
