#include <sys/types.h>
#include <string.h>
#include <sys/shm.h>
#include <errno.h>
#include "debug.h"

#define SHM_SUCCESS 0
#define SHM_FAILURE -1
#define SHM_SIZE 1024

char *shem_get_addr(int shmid);
int shm_del(int shmid);
int shm_new(unsigned char PROJID, size_t size);
void shm_write(char *shmaddr, char *buf);
void shm_read(char *buf, char *shmaddr);
void shm_data_init(char *shmaddr);
int shm_detach(char *shmaddr);
