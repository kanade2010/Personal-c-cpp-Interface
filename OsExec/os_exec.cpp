#include <stdio.h>
#include <iostream>
#include <string.h>

std::string exec(const char *cmd)  
 {    
 	std::string res;

    FILE *pp = popen(cmd, "r"); //建立管道  
    if (!pp)  
    {  
        return "";  
    }

    char tmp[1024];//设置一个合适的长度，以存储每一行输出  
    while (fgets(tmp, sizeof(tmp), pp) != NULL)  
    {  
        if (tmp[strlen(tmp) - 1] == '\n')  
        {  
            tmp[strlen(tmp) - 1] = '\0'; //去除换行符  
        }  
        res.append(tmp + std::string(" "));
    }
    res = {res, 0, res.size() - 1};

    pclose(pp); //关闭管道
    return std::move(res);  
}


int main(){
	std::cout << exec("ps -ef |grep sip_server |grep -v grep | awk '{print $2}'") << std::endl;
	std::cout << exec("ps -ef |grep http_server |grep -v grep | awk '{print $2}'") << std::endl;
	std::cout << exec("ps -ef |grep /bin/ |grep -v grep | awk '{print $2}'") << std::endl;
}