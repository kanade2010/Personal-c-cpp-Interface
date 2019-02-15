#include "msg_cache.hpp"
#include <iostream>

MsgCache::MsgCache()
{
  m_send_db.open("send.db");

  if(!m_send_db.tableExists("SendTable"))
  {
    std::cout << "CppSQLite3Table. CREATE table .\n";

    m_send_db.execDML("CREATE TABLE SendTable(msg TEXT);");
  }

}

MsgCache::~MsgCache()
{

}

void MsgCache::push(const std::string& msg)
{
  std::string sql = "INSERT INTO SendTable VALUES('" + msg + "');";

  std::lock_guard<std::mutex> lock(m_mutex);

  m_send_db.execDML(sql.c_str());

  std::cout << "MsgCache::push() msg: " << msg << std::endl;
}

const std::string MsgCache::pop()
{
  std::lock_guard<std::mutex> lock(m_mutex);

  CppSQLite3Query send_query = m_send_db.execQuery("SELECT msg FROM SendTable LIMIT 1 OFFSET 0;");

  if(send_query.eof())
  {
    return "";
  }


  std::string msg(send_query.getStringField(0));
  
  m_send_db.execDML("DELETE FROM SendTable WHERE msg = (SELECT msg FROM SendTable LIMIT 1 OFFSET 0)");

  std::cout << "MsgCache::pop() msg :" << msg << std::endl;
  std::cout << "MsgCache::pop() end." << std::endl;

}
