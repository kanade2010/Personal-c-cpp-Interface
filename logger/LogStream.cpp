#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include "LogStream.hh"

LogStream& LogStream::operator<<(bool v){
	if(v)
		m_buffer.append("true", 4);
	else
		m_buffer.append("false", 5);
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

const char digits[] = "9876543210123456789";
const char* zero = digits + 9;

//convert to str
template<typename T>
size_t convert(char buf[], T value){
	T i = value;
	char *p = buf;

	do{
		int lsd = static_cast<int>(i % 10);
		i /= 10;
		*p++ = zero[lsd];
	} while(i != 0);

	if(value < 0){
		*p++ = '-';
	}

	*p = '\0';
	std::reverse(buf, p);

	return p - buf;
}

template<typename T>
void LogStream::formatInteger(T v)
{
	if(m_buffer.avail() >= kMaxNumericSize){
		size_t len = convert(m_buffer.current(), v);
		m_buffer.add(len);
	}
}

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