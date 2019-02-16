#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "CppSQLite3.h"
#include <iostream>
#include <string>
#include "msg_cache.hpp"

int main()
{
	MsgCache st;

	st.push("{\"eventTime\":\"\",\"data\":{\"id\":54785,\"type\":\"Undefine\",\"lat\":\"\",\"lon\":\"\",\"trip_sn\":123456781}}");
	st.push("{\"eventTime\":\"\",\"data\":{\"id\":54785,\"type\":\"Undefine\",\"lat\":\"\",\"lon\":\"\",\"trip_sn\":123456782}}");
	st.push("{\"eventTime\":\"\",\"data\":{\"id\":54785,\"type\":\"Undefine\",\"lat\":\"\",\"lon\":\"\",\"trip_sn\":123456783}}");

	std::cout << st.pop() << std::endl;
	//std::cout << st.pop() << std::endl;
	//std::cout << st.pop() << std::endl;





/*
	CppSQLite3DB m_send_db;

	m_send_db.open("send.db");

  std::cout << "CppSQLite3Table. open send.db successful\n";

    // 判断表名是否已经存在
    if(!m_send_db.tableExists("SendTable"))
    {
        // 不存在,新建表Customers
          std::cout << "CppSQLite3Table. CREATE table .\n";

        m_send_db.execDML("CREATE TABLE SendTable(msg TEXT);");
    }

        std::cout << "CppSQLite3Table. insert table 1.\n";

    std::string msg = "hello world ";
  	std::string sql = "INSERT INTO SendTable VALUES('" + msg + "');";

    m_send_db.execDML(sql.c_str());

        std::cout << "CppSQLite3Table. insert table 2.\n";

    m_send_db.execDML("INSERT INTO SendTable VALUES('333 South Lake Drive');");


    CppSQLite3Query m_send_query = m_send_db.execQuery("SELECT msg FROM SendTable LIMIT 1 OFFSET 0;");

    if(!m_send_query.eof())
    {
    	std::cout << m_send_query.numFields() << "\n";
    	std::cout << m_send_query.getStringField(0) << std::endl;
    	//std::cout << m_send_query.getStringField(1);
    }
    
    m_send_db.execDML("DELETE FROM SendTable WHERE msg = (SELECT msg FROM SendTable LIMIT 1 OFFSET 0)");
    std::cout << "CppSQLite3Table. delete table 1.\n";

    m_send_query = m_send_db.execQuery("SELECT msg FROM SendTable LIMIT 1 OFFSET 0;");

    if(!m_send_query.eof())
    {
    	std::cout << m_send_query.numFields() << "\n";
    	std::cout << m_send_query.getStringField(0) << std::endl;
    	//std::cout << m_send_query.getStringField(1);
    }
*/
	/*printf("SQLiteVersion : %s\n", canStudyDB.SQLiteVersion());
	printf("%s\n", canStudyDB.tableExists("candata") == true?"exists":"inexists");
	CppSQLite3Table canStudyTable(canStudyDB.getTable("select * from can_data limit 10"));

	printf("CppSQLite3Table.numRows() : %d\n", canStudyTable.numRows());
	printf("CppSQLite3Table.fieldName(0) : %s\n", canStudyTable.fieldName(0));
	printf("CppSQLite3Table.fieldName(1) : %s\n", canStudyTable.fieldName(1));
	printf("CppSQLite3Table.fieldName(2) : %s\n", canStudyTable.fieldName(2));
	printf("CppSQLite3Table.fieldName(3) : %s\n", canStudyTable.fieldName(3));
	printf("CppSQLite3Table.getIntField(0) : %d\n", canStudyTable.getIntField(0));
	canStudyTable.setRow(1);
	printf("CppSQLite3Table.getIntField(0) : %d\n", canStudyTable.getIntField(0));

	CppSQLite3Query canStudtQuery(canStudyDB.execQuery("select * from can_data limit 10"));
	int len;
	const unsigned char* p = canStudtQuery.getBlobField("note", len);
	for(int i =0; i < len; i++)
		printf("%02x", p[i]);
	printf("\n");*/


}

