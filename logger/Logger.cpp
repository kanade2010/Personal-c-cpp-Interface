#include <time.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include "TimeStamp.hh"
#include "Logger.hh"

__thread char t_time[64];
__thread time_t t_lastSecond;

Logger::LogLevel g_logLevel = Logger::INFO;

void Logger::setLogLevel(LogLevel level){
	g_logLevel = level;
}

Logger::LogLevel Logger::logLevel(){
	return g_logLevel;
}

const char* LogLevelName[Logger::NUM_LOG_LEVELS] =
{
	"TRACE ",
	"DEBUG ",
	"INFO  ",
	"WARN  ",
	"ERROR ",
	"FATAL ",
};

// helper class for known string length at compile time
class T
{
 public:
  T(const char* str, unsigned len)
    :m_str(str),
     m_len(len)
  {
    assert(strlen(str) == m_len);
  }

  const char* m_str;
  const unsigned m_len;
};


Logger::Logger(SourceFile file, int line, LogLevel level, const char* func)
  : m_impl(level, 0, file, line)
{
  m_impl.m_stream << func << ' ';
}

Logger::~Logger(){
	m_impl.finish();
}

Logger::Impl::Impl(LogLevel level, int old_errno, const SourceFile& file, int line)
	: m_time(TimeStamp::now()),
	  m_stream(),
	  m_level(level),
	  m_fileBaseName(file),
	  m_line(line)
{
	formatTime();
	m_stream << LogLevelName[level] << ' ';
  /*if (savedErrno != 0)
  {
    stream_ << strerror_tl(savedErrno) << " (errno=" << savedErrno << ") ";
  }*/
}

void Logger::Impl::finish()
{
  m_stream << " - " << m_fileBaseName.m_data << ':' << m_line << '\n';
}

void Logger::Impl::formatTime()
{	
	int64_t microSecondsSinceEpoch = m_time.microSecondsSinceEpoch();
	time_t seconds = static_cast<time_t>(microSecondsSinceEpoch / TimeStamp::kMicroSecondsPerSecond);
	int microseconds = static_cast<int>(microSecondsSinceEpoch % TimeStamp::kMicroSecondsPerSecond);
	if (seconds != t_lastSecond){
		t_lastSecond = seconds;
		struct tm tm_time;

		::gmtime_r(&seconds, &tm_time); // FIXME TimeZone::fromUtcTime

		int len = snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d",
		tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
		tm_time.tm_hour + 8, tm_time.tm_min, tm_time.tm_sec);
		assert(len == 17); (void)len;
	}

	Fmt us(".%06d ", microseconds);
	assert(us.length() == 8);
	m_stream << t_time << us.data();
}

