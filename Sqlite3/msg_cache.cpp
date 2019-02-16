#include "msg_cache.hpp"
#include <iostream>

static const std::string kCacheTable = "CacheTable";

MsgCache::MsgCache()
{
  m_send_db.open("send.db");

  if(!m_send_db.tableExists("CacheTable"))
  {
    std::cout << "CppSQLite3Table. CREATE table .\n";
    m_size = 0;
    m_send_db.execDML("CREATE TABLE CacheTable(msg TEXT);");
  } else {
    CppSQLite3Query send_query = m_send_db.execQuery("SELECT COUNT(*) FROM CacheTable;");
    m_size = send_query.getInt64Field(0);
    std::cout << "table size " << m_size << "\n";
  }

}


MsgCache::~MsgCache()
{
  m_send_db.close();
}

void MsgCache::push(const std::string& msg)
{
  std::string sql = "INSERT INTO CacheTable (msg) VALUES('" + msg + "');";

  std::lock_guard<std::mutex> lock(m_mutex);

  m_send_db.execDML(sql.c_str());

  m_size++;

  std::cout << "MsgCache::push() msg: " << msg << std::endl;
}

const std::string MsgCache::pop()
{
  std::lock_guard<std::mutex> lock(m_mutex);

  CppSQLite3Query send_query = m_send_db.execQuery("SELECT msg FROM CacheTable LIMIT 1 OFFSET 0;");

  if(send_query.eof())
  {
    return "";
  }

  std::string msg(send_query.getStringField(0));
  
  m_send_db.execDML("DELETE FROM CacheTable WHERE rowid = (SELECT rowid FROM CacheTable LIMIT 1)");

  std::cout << "MsgCache::pop() msg :" << msg << std::endl;
 
  m_size--;

  return msg;
}
