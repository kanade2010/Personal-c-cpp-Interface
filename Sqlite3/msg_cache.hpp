#include "CppSQLite3.h"
#include <mutex>
#include <string>

class MsgCache{
public:
  typedef int64_t size_type;

public:
  MsgCache();
  ~MsgCache();

  void push(const std::string& msg);
  const std::string pop();
  size_type size() { std::lock_guard<std::mutex> lock(m_mutex); return m_size; }

private:
  MsgCache(const MsgCache&);
  const MsgCache& operator=(const MsgCache&);

  CppSQLite3DB m_send_db;

  std::mutex m_mutex; // guard send db;

  size_type m_size;
};