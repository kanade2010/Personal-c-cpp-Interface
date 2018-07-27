#include <sys/select.h>
#include <unistd.h>     
#include <stdio.h>  
#include <stdlib.h>   
#include <termios.h>     
#include <fcntl.h>     
#include <string.h>     
#include <time.h>
#include <errno.h>

#define GSENSOR_DEV "/dev/ttyUSB0"

int serial_init();
int serial_conf(int fd, int nspeed);
int serial_open(char *path);
int serial_read(int fd, void *buf, int len, int timeout);
