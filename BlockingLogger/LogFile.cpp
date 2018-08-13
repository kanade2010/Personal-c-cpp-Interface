#include "LogFile.hh"
#include "FileUtil.hh"

LogFile::LogFile(const std::string& filePath, off_t rollSize, bool threadSafe, int flushInterval)
	:m_filePath(filePath),
	 m_roolSize(rollSize),
	 m_mutex(threadSafe ? new MutexLock : NULL),
	 m_rollCnt(-1),
	 m_flushInterval(flushInterval){
	//assert(filePath.find('/') == std::string::npos);
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

	if(m_file->writtenBytes() > m_roolSize){
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
	std::string fileNameOld;
	std::string fileNameNew;

	for(int i = m_rollCnt; i >= 0; --i ){
		fileNameOld = getlogFileName(m_filePath, i);
		fileNameNew = getlogFileName(m_filePath, i + 1);
		::rename(fileNameOld.c_str(), fileNameNew.c_str());
		if(i == 4) ::remove(fileNameNew.c_str());
	}

	m_file.reset(new FileUtil::AppendFile(m_filePath));

	if(m_rollCnt < 4)
		m_rollCnt++;
	return true;
}

std::string LogFile::getlogFileName(const std::string& baseName, int suffix){
	std::string fileName;
	fileName.reserve(baseName.size() + 3);
	fileName = baseName;

	char tmp[4];
	if(suffix != 0)
		snprintf(tmp, sizeof(tmp), ".%1d", suffix);

	fileName += tmp;
	return fileName;
}










