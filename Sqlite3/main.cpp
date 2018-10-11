#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "CppSQLite3.h"

int main()
{
	CppSQLite3DB canStudyDB;
	canStudyDB.open("candata");
	printf("SQLiteVersion : %s\n", canStudyDB.SQLiteVersion());
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
	printf("\n");

}

