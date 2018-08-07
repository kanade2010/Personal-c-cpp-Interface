#include "LogFile.hh"
#include "FileUtil.hh"

LogFile::LogFile(const std::string& baseName, bool threadSafe, int flushInterval)
	:m_baseName(baseName),
	 m_roolSize(rollSize),
	 m_mutex(threadSafe ? new MutexLock : NULL),
	 m_flushInterval(flushInterval){
	//assert(baseName.find('/') == std::string::npos);
	rollFile();
}

LogFile::~LogFile(){
}

void LogFile::append(const char* logline, int len){
	if(m_mutex.get()){
		MutexLockGuard lock(*m_mutex);
		append_unlocked(logline, len);
	}
	else{
		append_unlocked(logline, len);
	}
}

void LogFile::append_unlocked(const char* logline, int len){
	m_file->append(logline, len);

	if(m_file->writtenBytes > m_roolSize){
		rollFile();
	}
}

void LogFile::flush(){
	if(m_mutex.get()){
		MutexLockGuard lock(*m_mutex);
		m_file->flush();
	}
	else{
		m_file->flush();
	}
}

bool LogFile::rollFile(){
	std::string filename = "./test.log";

	m_file.reset(new FileUtil::AppendFile(filename));

	return true;
}
