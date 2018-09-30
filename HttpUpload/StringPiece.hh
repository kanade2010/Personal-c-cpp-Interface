#ifndef _STRINGPIECE_HH
#define _STRINGPIECE_HH

#include <string>

// For passing C-style string argument to a function.
class StringArg
{
public:
	StringArg(const char *str):m_str(str){
	}
	
	StringArg(const std::string& str):m_str(str.c_str()){
	}

	const char* c_str() const{ return m_str;}

private:
	const char* m_str;
};

#endif