#include <stdio.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <errno.h>

#define SEM_SUCCESS 0
#define SEM_FAILURE -1
#define SEM_EXIST -2

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
};


int sem_get(unsigned char proj_id);
int sem_new(unsigned char proj_id, int val);
int sem_del(int semid);
int sem_p();
int sem_v();




