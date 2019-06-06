// duration constructor
#include <iostream>
#include <ratio>
#include <chrono>
#include <time.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <chrono>
/*int main ()
{
  typedef std::chrono::duration<int> seconds_type;
  typedef std::chrono::duration<int,std::milli> milliseconds_type;
  typedef std::chrono::duration<int,std::ratio<60*60>> hours_type;
 
  hours_type h_oneday (24);                  // 24h
  seconds_type s_oneday (60*60*24);          // 86400s
  milliseconds_type ms_oneday (s_oneday);    // 86400000ms
 
  seconds_type s_onehour (60*60);            // 3600s
//hours_type h_onehour (s_onehour);          // NOT VALID (type truncates), use:
  hours_type h_onehour (std::chrono::duration_cast<hours_type>(s_onehour));
  milliseconds_type ms_onehour (s_onehour);  // 3600000ms (ok, no type truncation)
 
  std::cout << ms_onehour.count() << "ms in 1h" << std::endl;
 
  return 0;
}*/
 
//duration还有一个成员函数count()返回Rep类型的Period数量，看代码：

// duration::count
#include <iostream>     // std::cout
#include <chrono>       // std::chrono::seconds, std::chrono::milliseconds
                        // std::chrono::duration_cast
 
// 获取系统当前时间
std::string getCurrentSystemTime()
{
  auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  struct tm* ptm = localtime(&tt);
  char date[64] = { 0 };
  sprintf(date, "%d-%02d-%02d %02d:%02d:%02d",
         (int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday,
         (int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec);

  return std::string(date);
}

volatile int sink;
int main()
{
  using namespace std::chrono;
  // std::chrono::milliseconds is an instatiation of std::chrono::duration:
  milliseconds foo (1000); // 1 second
  foo*=60;
 
  std::cout << "duration (in periods): ";
  std::cout << foo.count() << " milliseconds.\n";
 
  std::cout << "duration (in seconds): ";
  std::cout << foo.count() * milliseconds::period::num / milliseconds::period::den;
  std::cout << " seconds.\n";
 

  nanoseconds nas(1000);
  std::cout << "duration (in periods): ";
  std::cout << nas.count() << " nanoseconds.\n";
  std::cout << getCurrentSystemTime() << "\n";
  
  //high_resolution_clock
  
      for (auto size = 1ull; size < 1000000000ull; size *= 100) {
        // record start time
        auto start = std::chrono::high_resolution_clock::now();
        // do some work
        std::vector<int> v(size, 42);
        sink = std::accumulate(v.begin(), v.end(), 0u); // make sure it's a side effect
        // record end time
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end-start;
        std::cout << "Time to fill and iterate a vector of " 
                  << size << " ints : " << diff.count() << " s\n";
    }
	
	
	
  return 0;
}