#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

void printTime()
{
  struct timeval tv;
  gettimeofday(&tv, 0);
  __int64_t seconds = tv.tv_sec;
  __int64_t microSecondsSinceEpoch = seconds * 1000 * 1000 + tv.tv_usec;
  seconds = (time_t)(microSecondsSinceEpoch / (1000 * 1000));
  int microseconds = (int)(microSecondsSinceEpoch % (1000 * 1000));
  struct tm tm_time;

  ::gmtime_r(&seconds, &tm_time); // FIXME TimeZone::fromUtcTime

  int len = printf("%4d-%02d-%02d %02d:%02d:%02d",
  tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
  tm_time.tm_hour + 8, tm_time.tm_min, tm_time.tm_sec);

  printf(":%d\n", microseconds);
}


int main()
{

  printTime();
  sleep(1);
  printTime();
  sleep(1);
  printTime();

  return 0;
}