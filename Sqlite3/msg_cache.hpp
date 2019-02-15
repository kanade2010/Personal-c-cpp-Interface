#include "CppSQLite3.h"
#include <mutex>
#include <string>

class MsgCache{
public:
  MsgCache();
  ~MsgCache();

  void push(const std::string& msg);
  const std::string pop();

private:
  MsgCache(const MsgCache&);
  const MsgCache& operator=(const MsgCache&);

  CppSQLite3DB m_send_db;

  std::mutex m_mutex; // guard send db;
};