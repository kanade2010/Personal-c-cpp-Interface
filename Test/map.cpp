#include <map>
#include <string>
#include <iostream>

std::map<int, std::string>::value_type init_value[] =
{
  std::map<int, std::string>::value_type(0, "GET"),

  std::map<int, std::string>::value_type(1, "POST")
};

std::map<int, std::string> m_strs(init_value, init_value + (sizeof init_value / sizeof init_value[0]));

int mian()
{
  std::map<int, std::string> StringMap;

  std::map<int, std::string>::const_iterator str = m_strs.find(1);

  std::cout << " std: " << str->second << std::endl;

  return 0;
}