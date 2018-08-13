#include "FileUtil.hh"
#include <assert.h>
#include <stdio.h>
#include <string.h>

FileUtil::AppendFile::AppendFile(StringArg filename)
	:m_fp(::fopen(filename.c_str(), "ae")), // 'e' for O_CLOEXEC
	 m_writtenBytes(0)
{
	assert(m_fp);
	::setbuffer(m_fp, m_buffer, sizeof(m_buffer));
}

FileUtil::AppendFile::~AppendFile(){
	::fclose(m_fp);
}

void FileUtil::AppendFile::append(const char* logline, const size_t len){
	size_t nread = write(logline, len);
	size_t remain = len - nread;
	while(remain > 0){
		size_t n = write(logline + n, remain);
		if(0 == n){
			int err = ferror(m_fp);
			if(err){
				fprintf(stderr, "AppendFile::append failed : %s\n", strerror(err));
			}
			break;
		}
		nread += n;
		remain = len - nread;
	}

	m_writtenBytes += len;
}

size_t FileUtil::AppendFile::write(const char* logline, const size_t len){
	return ::fwrite_unlocked(logline, 1, len, m_fp);
}

void FileUtil::AppendFile::flush(){
	::fflush(m_fp);
}