#include <deque>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
//---------------------------TimeS-------------------

class Timestamp {
public:
//
// Constucts an invalid Timestamp.
//
	Timestamp()
	: m_microSecondsSinceEpoch(0){
	}

//
// Constucts a Timestamp at specific time
//
// @param microSecondsSinceEpoch
	explicit Timestamp(int64_t microSecondsSinceEpochArg)
	: m_microSecondsSinceEpoch(microSecondsSinceEpochArg){
	}

	int64_t microSecondsSinceEpoch() const { return m_microSecondsSinceEpoch; }

//
// Get time of now.
//
	static Timestamp now();

	static const int kMicroSecondsPerSecond = 1000 * 1000;

private:
	int64_t m_microSecondsSinceEpoch;
};

Timestamp Timestamp::now(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	int64_t seconds = tv.tv_sec;
	return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

//---------------------------LogStream-------------------
/*
class FixdBuffer
{
public:
	FixdBuffer():m_cur(m_data){
		setCookie(cookieStart);
	}
	~FixdBuffer(){
		setCookie(cookieEnd)
	}
	
private:
	const char* end() const{
		return m_data + 
	}

	void (*m_cookie)();
	char m_data[SIZE];
	char* m_cur;
};*/


class LogStream{
public:
	typedef LogStream self;
	self& operator<<(bool v);

	self& operator<<(short);
	self& operator<<(unsigned short);
	self& operator<<(int);
	self& operator<<(unsigned int);
	self& operator<<(long);
	self& operator<<(unsigned long);
	self& operator<<(long long);
	self& operator<<(unsigned long long);

	self& operator<<(const void*);

	self& operator<<(float v);
	self& operator<<(double);

	self& operator<<(char v);
	self& operator<<(const char *);

	template<typename T>
	void formatInteger(T v);

};

class Fmt{
public:
	template<typename T>
	Fmt(const char* fmt, T val);

	const char* data() const { return m_buf; }
	int length() const { return m_length; }

private:
	char m_buf[32];
	int m_length;
};

//`````````LogStream``````````````````````````````````````````````

template<typename T>
Fmt::Fmt(const char* fmt, T val)
{
	m_length = snprintf(m_buf, sizeof(m_buf), fmt, val);
	assert(static_cast<size_t>(m_length) < sizeof(m_buf));
}

// Explicit instantiations

template Fmt::Fmt(const char* fmt, char);

template Fmt::Fmt(const char* fmt, short);
template Fmt::Fmt(const char* fmt, unsigned short);
template Fmt::Fmt(const char* fmt, int);
template Fmt::Fmt(const char* fmt, unsigned int);
template Fmt::Fmt(const char* fmt, long);
template Fmt::Fmt(const char* fmt, unsigned long);
template Fmt::Fmt(const char* fmt, long long);
template Fmt::Fmt(const char* fmt, unsigned long long);

template Fmt::Fmt(const char* fmt, float);
template Fmt::Fmt(const char* fmt, double);

LogStream& LogStream::operator<<(bool v){
	printf("%s\n", v?"true":"false");
	return *this;
}


LogStream& LogStream::operator<<(short v){
	*this << static_cast<int>(v);
	return *this;
}

LogStream& LogStream::operator<<(unsigned short v)
{
  *this << static_cast<unsigned int>(v);
  return *this;
}

LogStream& LogStream::operator<<(int v)
{
  formatInteger(v);
  return *this;
}

LogStream& LogStream::operator<<(unsigned int v)
{
  formatInteger(v);
  return *this;
}

LogStream& LogStream::operator<<(long v)
{
  formatInteger(v);
  return *this;
}

LogStream& LogStream::operator<<(unsigned long v)
{
  formatInteger(v);
  return *this;
}

LogStream& LogStream::operator<<(long long v)
{
  formatInteger(v);
  return *this;
}

LogStream& LogStream::operator<<(unsigned long long v)
{
  formatInteger(v);
  return *this;
}

LogStream& LogStream::operator<<(const void*){
	printf("undefine");
}

LogStream& LogStream::operator<<(float v)
  {
    *this << static_cast<double>(v);
    return *this;
  }

LogStream& LogStream::operator<<(double v){
	printf("%lf", v);
}

LogStream& LogStream::operator<<(char v){
	printf("%c", v);
}

LogStream& LogStream::operator<<(const char *v){
	printf("%s", v);
}

template<typename T>
void LogStream::formatInteger(T v)
{
	printf("%d", v);
}



//`````````Logger``````````````````````````````````````````````
//

class Logger
{
public:
	enum LogLevel
	{
		TRACE,
		DEBUG,
		INFO,
		WARN,
		ERROR,
		FATAL,
		NUM_LOG_LEVELS,
	};

//compile time calculation of basename of source file
	class SourceFile
	{
	public:
		template<int N>
		inline SourceFile(const char (&arr)[N])
			:m_data(arr),
			 m_size(N-1){
			const char* slash = strrchr(m_data, '/'); // builtin function
			if (slash){
				m_data = slash + 1;
				m_size -= static_cast<int>(m_data - arr);
			}
		}

		explicit SourceFile(const char* filename)
			: m_data(filename){
			const char* slash = strrchr(filename, '/');
			if (slash){
				m_data = slash + 1;
			}
			m_size = static_cast<int>(strlen(m_data));
		}

		const char* m_data;
		int m_size;
	};

	Logger(SourceFile file, int line, LogLevel level, const char* func);
	~Logger();

	static void setLogLevel(LogLevel level);
	static LogLevel logLevel();

	LogStream& stream() { return m_impl.m_stream; }

private:

	class Impl
	{
	public:
		typedef Logger::LogLevel LogLevel;
		Impl(LogLevel level, int old_errno, const SourceFile& file, int line);
		void formatTime();
		void finish();

		Timestamp m_time;
		LogStream m_stream;
		LogLevel m_level;
		int m_line;
		SourceFile m_fileBaseName;
	};

	Impl m_impl;

};

//--------------logger-impl----------
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
	: m_time(Timestamp::now()),
	  m_stream(),
	  m_level(level),
	  m_fileBaseName(file),
	  m_line(line)
{
	formatTime();
	m_stream << LogLevelName[level] << ' ' << m_fileBaseName.m_data << ' ';
  /*if (savedErrno != 0)
  {
    stream_ << strerror_tl(savedErrno) << " (errno=" << savedErrno << ") ";
  }*/
}

void Logger::Impl::finish()
{
  m_stream << " - " /*<< m_baseName*/ << ':' << m_line << '\n';
}

void Logger::Impl::formatTime()
{	
	int64_t microSecondsSinceEpoch = m_time.microSecondsSinceEpoch();
	time_t seconds = static_cast<time_t>(microSecondsSinceEpoch / Timestamp::kMicroSecondsPerSecond);
	int microseconds = static_cast<int>(microSecondsSinceEpoch % Timestamp::kMicroSecondsPerSecond);
	if (seconds != t_lastSecond){
		t_lastSecond = seconds;
		struct tm tm_time;

		::gmtime_r(&seconds, &tm_time); // FIXME TimeZone::fromUtcTime

		int len = snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d",
		tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
		tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
		assert(len == 17); (void)len;
	}

	Fmt us(".%06d ", microseconds);
	assert(us.length() == 8);
	m_stream << t_time << us.data();
}

// CAUTION: do not write:
//
// if (good)
//   LOG_INFO << "Good news";
// else
//   LOG_WARN << "Bad news";
//
// this expends to
//
// if (good)
//   if (logging_INFO)
//     logInfoStream << "Good news";
//   else
//     logWarnStream << "Bad news";
//
#define LOG_TRACE if (Logger::logLevel() <= Logger::TRACE) \
	Logger(__FILE__, __LINE__, Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (Logger::logLevel() <= Logger::DEBUG) \
	Logger(__FILE__, __LINE__, Logger::DEBUG, __func__).stream()
#define LOG_INFO if (Logger::logLevel() <= Logger::INFO) \
	Logger(__FILE__, __LINE__).stream()
//#define LOG_WARN Logger(__FILE__, __LINE__, Logger::WARN).stream()
#define LOG_WARN Logger(__FILE__, __LINE__, Logger::WARN, __func__).stream()
#define LOG_ERROR Logger(__FILE__, __LINE__, Logger::ERROR).stream()
#define LOG_FATAL Logger(__FILE__, __LINE__, Logger::FATAL).stream()
#define LOG_SYSERR Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL Logger(__FILE__, __LINE__, true).stream()


int main()
{
	/*
	std::deque<int> t;
		int i;
	for(i=0;i<10;i++)
	{
		t.push_back(i);
	}
	
	for(i=0;i<10;i++)
	{	
		printf("%d ", t[i]);
	}

	printf("\n");
*/

	LogStream re;
	int a = 5;
	bool b = true;
	float c = 3.141592654;
	void *vop = &c;
	//re<< a << 'a' << b << c << vop << "fuck you";


	Logger::LogLevel level;
	//re<< Logger::DEBUG << Logger::FATAL<<"\n";

	for(int i = 0; i< 200 ; i++){
	LOG_WARN << "just test";
	LOG_WARN << "just test";
	LOG_WARN << "just test";
	LOG_WARN << "just test";
	LOG_WARN << "just test";
}

	return 0;
}
