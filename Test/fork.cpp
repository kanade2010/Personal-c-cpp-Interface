#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main()
{
   int i=0;
   for(i=0;i<3000;i++){

       pid_t fpid=fork();
       if(fpid==0){
           printf("run test_client\n");
           execl("/home/ailumiyana/Git_Project/Personal-c-cpp-Interface/Test/test_client", NULL);
       }
       else{
           printf("father\n");
       }
   
   }

   getchar();
}
