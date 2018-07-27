#include "sem.h"
#include "debug.h"

#define IPCKEY_PATH "/"
#define SEM_NUMS 1
#define SEM_OPS_NUM 1
#define ACCESS_BIT 0666

#define TAG "SemInterface"

int sem_new(unsigned char projid, int init_val)
{
	key_t key;
	int semid;
	union semun sem_union;

	key = ftok(IPCKEY_PATH, projid);
	if(key < 0)
	{
		LOG_E("ftok error: %s\n", strerror(errno));
		return SEM_FAILURE;
	}

	semid = semget(key, SEM_NUMS, ACCESS_BIT|IPC_CREAT|IPC_EXCL);
	if(semid < 0)
	{
		if(errno == EEXIST)
		{
		LOG_E("sem exist: %s\n", strerror(errno));
		return SEM_EXIST;
		}
		LOG_E("create sme error: %s\n", strerror(errno));
		return SEM_FAILURE;
	}

	sem_union.val = init_val;
	if((semctl(semid, 0, SETVAL, sem_union)) < 0)
	{
		LOG_E("set sem val error: %s\n", strerror(errno));
		return SEM_FAILURE;
	}

	return semid;
}


int sem_get(unsigned char proj_id)
{
	key_t key;
	int semid;
	union semun sem_union;

	key = ftok(IPCKEY_PATH, proj_id);
	if(key < 0)
	{
		LOG_E("ftok error: %s\n", strerror(errno));
		return SEM_FAILURE;
	}

	semid = semget(key, SEM_NUMS, ACCESS_BIT);
	if(semid < 0)
	{
		LOG_E("create sme error: %s\n", strerror(errno));
		return SEM_FAILURE;
	}

	return semid;
}


int sem_p(int semid)
{
	struct sembuf sem_buf;
	sem_buf.sem_num = 0;
	sem_buf.sem_op = -1;
	sem_buf.sem_flg = SEM_UNDO;

	if(semop(semid, &sem_buf, SEM_OPS_NUM) < 0)
	{
		LOG_E("sem P opration error: %s", strerror(errno));
		return SEM_FAILURE;
	}

	return SEM_SUCCESS;
}


int sem_v(int semid)
{
	struct sembuf sem_buf;
	sem_buf.sem_num = 0;
	sem_buf.sem_op = 1;
	sem_buf.sem_flg = SEM_UNDO;

	if(semop(semid, &sem_buf, SEM_OPS_NUM) < 0)
	{
		LOG_E("sem V opration error: %s", strerror(errno));
		return SEM_FAILURE;
	}

	return SEM_SUCCESS;
}


int sem_del(int semid)
{
	union semun sem_union;
	if(semctl(semid, 0, IPC_RMID, sem_union) < 0)  
	{
		LOG_E("remove sem error: %s\n", strerror(errno));
		return SEM_FAILURE;
	}

	return SEM_SUCCESS;
}





