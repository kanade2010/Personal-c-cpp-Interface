#include "shmem.h"
#include "debug.h"

#define IPCKEY_PATH "/"
#define ACCESS_BIT 0666
#define INIT_DATA "InitData"

#define TAG "ShareMemory"

int shm_new(unsigned char PROJID, size_t size)
{
	int shmid;
	key_t key;

	key = ftok(IPCKEY_PATH, PROJID);
	if(key < 0)
	{
		LOG_E("get IPC key error: %s\n", strerror(errno));
		return SHM_FAILURE;
	}

	shmid = shmget(key, size, ACCESS_BIT|IPC_CREAT);
	if(shmid < 0)
	{
		LOG_E("get share mem error: %s\n", strerror(errno));
		return SHM_FAILURE;
	}

	return shmid;
}

char *shem_get_addr(int shmid)
{
	char *p;

	if((p = (shmat(shmid, NULL, 0))) == (char *)-1)
	{
		LOG_E("get share mem addr error: %s\n", strerror(errno));
		return NULL;
	}

	return p; 
}

int shm_del(int shmid)
{	
	if(shmctl(shmid, IPC_RMID, NULL) < 0)
	{
		LOG_E("remove share mem error: %s\n", strerror(errno));
		return SHM_FAILURE;
	}

	return SHM_SUCCESS;
}

int shm_detach(char *shmaddr)
{
	if(shmdt(shmaddr) < 0)
	{
		LOG_E("share mem detach error: %s\n", strerror(errno));
		return SHM_FAILURE;
	}
	return SHM_SUCCESS;
}

void shm_read(char *buf, char *shmaddr)
{	
	strncpy(buf, shmaddr, strlen(shmaddr) + 1);
}

void shm_write(char *shmaddr, char *buf)
{	
	strncpy(shmaddr, buf, strlen(buf) + 1);
}

void shm_data_init(char *shmaddr)
{
	memset(shmaddr, 0, SHM_SIZE);	
	//strncpy(shmaddr, INIT_DATA, strlen(INIT_DATA) + 1);
}

