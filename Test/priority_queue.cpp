#include <utility>
#include <queue>
#include <vector>
#include <iostream>

enum taskPriorityE { level0, level1, level2, };

typedef std::pair<taskPriorityE, int> TaskType;

class TaskCmp
{
public:
  bool operator()(const TaskType p1, const TaskType p2)
  {
      return p1.first > p2.first; //first的小值优先
  }
};

int main()
{
  std::priority_queue<TaskType, std::vector<TaskType>, TaskCmp> pqueue;

  pqueue.push(TaskType(level2, 8));
  pqueue.push(TaskType(level0, 10));
  pqueue.push(TaskType(level1, 9));
  pqueue.push(TaskType(level0, 6));
  pqueue.push(TaskType(level0, 5));


  std::cout << pqueue.top().first << std::endl;
  pqueue.pop();
  std::cout << pqueue.top().first << std::endl;
  std::cout << pqueue.top().second << std::endl;
  pqueue.push(TaskType(level0, 4));
  std::cout << pqueue.top().second << std::endl;
  pqueue.push(TaskType(level0, 3));
  pqueue.push(TaskType(level0, 5));
  std::cout << pqueue.top().second << std::endl;

}