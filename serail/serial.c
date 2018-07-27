#include "serial.h"
#include "debug.h"

#define TAG "Serial"

int serial_read(int fd, void *buf, int len, int timeout)
{
    int ret;
    int nread;
    fd_set readfd;
    struct timeval vtime;

    FD_ZERO(&readfd);
    FD_SET(fd, &readfd);

    vtime.tv_sec = timeout / 1000;
    vtime.tv_usec = (timeout % 1000) * 1000;

    ret = select(fd + 1, &readfd, NULL, NULL, &vtime);
    if(0 == ret)
    {
        LOG_D("select timeout\n");
        return 0;
    }
    else if(ret < 0)
    {
        LOG_E("select error %d : %s\n",errno, strerror(errno));
        return -1;
    }
    else
    {
        nread = read(fd, buf, len);
        if(nread < 0)
        {
            LOG_E("read error %d : %s\n", errno, strerror(errno));
            return -1;
        }

        return nread;
    }
}

int serial_open(char *path)
{

    int fd;

    fd = open(path,O_RDONLY | O_NOCTTY | O_NDELAY);

    if(fd < 0)
    {
        LOG_E("serial open error %d : %s\n", errno, strerror(errno));
        return -1;
    }

    return fd;
}



int serial_conf(int fd, int nspeed)
{

    struct termios old, new;    
    
    if(tcgetattr(fd,&old) != 0)    
    {    
        LOG_E("sget old attr error %d : %s\n", errno, strerror(errno));
        return -1;    
    }    
    
    switch(nspeed)
    {
        case 115200:
            cfsetispeed(&new, B115200);   
            cfsetospeed(&new, B115200);   //波特率
            break;
        case 9600:
        default:
            cfsetispeed(&new, B9600);   
            cfsetospeed(&new, B9600);
            LOG_D("B9600\n"); 
	    break;

    }
    /*八个数据位，不进行奇偶校验*/  
    new.c_cflag |= (CLOCAL | CREAD); //一般必设置的标志

    new.c_cflag &= ~CSIZE;
    new.c_cflag |= CS8;      //8数据位 
    //new.c_cflag |= CBAUD;	

    new.c_cflag &= ~PARENB;  //清除校验位
    //new.c_iflag &= ~INPCK;   //enable parity checking

    new.c_cflag &= ~CSTOPB;  //1个停止位
    new.c_cflag &= ~CRTSCTS;  //禁用硬件流控

    //new.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    //new.c_iflag &= ~(ICRNL | IXON | IXOFF | IXANY);
    //new.c_iflag &= ~(IXON | IXOFF | IXANY );
    new.c_iflag &= ~(ICRNL | IXON | INLCR | IGNCR | IXOFF | IUCLC | ISTRIP); //解决读二进制串口数据上报异常问题.

    /*设置等待时间和最小接收字符*/   
    new.c_cc[VTIME] = 0; //设置超时为15sec
    new.c_cc[VMIN] = 0; //Update the new and do it now  
    /*处理未接收字符*/
    
    tcflush(fd,TCIFLUSH);                           //清空输入缓存     
    
    if(tcsetattr(fd,TCSANOW,&new) != 0)    
    {    
        LOG_E("cfg serial error %d : %s\n",errno, strerror(errno));
        return -1;    
    }    
    
    return 0;    
}

int serial_init()
{
    int ret;
    int fd;
    
    fd =  serial_open(GSENSOR_DEV);
    if(fd < 0)
    {    
        LOG_E("file open error exit\n");    
        return -1;    
    }
    
    ret = serial_conf(fd, 9600);
    if(ret < 0)
    {
        LOG_E("serial config error exit\n");
        return -1;
    }

    return fd;
}
