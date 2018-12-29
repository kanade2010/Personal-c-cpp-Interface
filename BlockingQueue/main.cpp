#include "BlockingQueue.hh"
#include <thread>
#include <iostream>
#include <chrono>

BlockingQueue<int> IntQueue;

void task()
{
  int i = 0;
  //std::this_thread
  i = 100;
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  IntQueue.put(i);
}


int main()
{
  std::thread t1(task);

  int i = IntQueue.take();

  std::cout << i << std::endl;
  t1.join();

}