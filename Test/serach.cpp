#include <algorithm>

#include <cstdio>
#include <string.h>

int main()
{
  const char* str = "123456789\r\n123456\r\n123456";
  const char kCRLF[] = "\r\n";

  printf("%s\n", str);

  const char* crlf = std::search(str, str + 19, kCRLF, kCRLF + 2);

  printf("%d\n", crlf - str);






  str = str + 9 + 2;

  printf("%d %s\n",strlen(str), str);

  crlf = std::search(str, str + strlen(str) + 1, kCRLF, kCRLF + 2);

  printf("%d\n", crlf - str);


  str = str + 6 + 2;

  printf("%d %s\n",strlen(str), str);

  crlf = std::search(str, str + strlen(str) + 1, kCRLF, kCRLF + 2);

  printf("%d\n", crlf - str);
}