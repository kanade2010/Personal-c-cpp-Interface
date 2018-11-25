#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>

int main()
{
  int sock = socket(AF_INET, SOCK_DGRAM, 0);

  int type = -1;
  socklen_t typeLen = static_cast<socklen_t>(sizeof type);
  int ret = getsockopt(sock, SOL_SOCKET, SO_TYPE, &type, &typeLen);
  if(ret < 0)
  {
    printf("getsockopt error .\n");
    return 0;
  }

  if(type == SOCK_DGRAM) printf("SOCK_DGRAM\n");


}