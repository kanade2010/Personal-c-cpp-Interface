#include <map>
#include <string>
#include <iostream>
#include <stdint.h>

/*
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

*/
/*
//#include <utility>
#include <set>

int main()
{
  std::pair<int, std::string> test;
  test.first = 0;
  test.second = "Hello World";

  std::cout << test.first << ' ' << test.second << std::endl;

  test = std::make_pair(1, "Hello Test1");

  std::cout << test.first << ' ' << test.second << std::endl;

  return 0;
}

*/
#include <set>
/*
int main()
{

  std::set<int> myset = { 50, 20, 60, 10, 25 ,10, 10};
 
  std::cout << "myset contains:";
  for (std::set<int>::const_iterator it = myset.begin(); it != myset.end(); ++it)
    std::cout << ' ' << *it ;

  std::cout << std::endl;;
 
  std::set<int>::iterator it = myset.lower_bound(20);
  std::cout << *it << std::endl;

  return 0;

}

*/

typedef std::pair<int, uintptr_t> test_t;
typedef std::set<test_t> test_t_List;

int main()
{
  test_t mypair1 = std::make_pair(10, 10000);

  test_t mypair2 = std::make_pair(10, 10001);

  test_t mypair3 = std::make_pair(20, 1000);

  test_t mypair4 = std::make_pair(20, 10000);

  test_t_List myset = {mypair3, mypair2, mypair1, mypair4};

  for (std::set<test_t>::const_iterator it = myset.begin(); it != myset.end(); ++it)
  std::cout << "    " << (*it).first <<'|' << (*it).second;
  std::cout << std::endl;

  test_t dfind = std::make_pair(5, UINTPTR_MAX);

  std::set<test_t>::iterator it = myset.lower_bound(dfind);
  std::cout << "    " << (*it).first <<'|' << (*it).second;
  std::cout << std::endl;

  return 0;
}