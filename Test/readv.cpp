#include <sys/uio.h>
#include <stdio.h>


int main()
{
  char buf1[2] = {0};
  char buf2[24] = {0};

  struct iovec vec[2];
  vec[0].iov_base = buf1;
  vec[0].iov_len = sizeof buf1;
  vec[1].iov_base = buf2;
  vec[1].iov_len = sizeof buf2;

  const ssize_t n = readv(0/*stdin*/, vec, 2 );

  if (n < 0)
  {
    printf("read error\n");
    return -1;
  }
  else if (n <= sizeof buf1)
  {
   printf("read buf1 %s \n", buf1);
    return 0;
  }
  else
  {
    printf("read buf1 %c%c \n", buf1[0], buf1[1]);
    printf("read buf2 %s \n", buf2);
    return 0;
  }

  return 0;
}