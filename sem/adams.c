#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <time.h>

#define pri(fmt, ...) do { 	printf("["__FILE__"][%s]-%d: " ,__FUNCTION__,__LINE__ );\
						printf(fmt, ##__VA_ARGS__);} while (0)
						

union semun
{
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	/* struct seminfo  *__buf; */  /* Buffer for IPC_INFO
	                                         (Linux-specific) */
};

int InitSem(int SemId)	//初始化信号量
{
	int ret;
	union semun sem;
	sem.val = 1;

	ret = semctl(SemId, 0, SETVAL, sem);
	if(-1 == ret)
		{
			perror("semctl Init error ");
			return -1;
		}
	return 0;
}

int DelSem(int SemId)	//删除信号量
{
	union semun sem;
	/* 	sem.val = 1; */

	int ret = semctl(SemId, 0, IPC_RMID, sem);
	if(-1 == ret)
		{
			perror("semctl Del error ");
			return -1;
		}
	return 0;
}


#if 0
/* Structure used for argument to `semop' to describe operations.  */
struct sembuf
{
	unsigned short int sem_num;   /* semaphore number */
	short int sem_op;             /* semaphore operation */
	short int sem_flg;            /* operation flag */
};

#endif


int PFunc(int SemId)		//申请资源 -1;
{
	struct sembuf s;
	s.sem_num = 0;
	s.sem_op = -1;
	s.sem_flg = SEM_UNDO;

	int ret = semop(SemId, &s, 1);
	if(-1 == ret)
		{
			perror("semop P error ");
			return 0;
		}
	pri("---------- \n");
	return 1;
}

int VFunc(int SemId)		//释放资源 +1;
{
	int ret;
	struct sembuf s;
	s.sem_num = 0;
	s.sem_op = 1;
	s.sem_flg = SEM_UNDO;

	ret = semop(SemId, &s, 1);
	if(-1 == ret)
		{
			perror("semop V error ");
			return -1;
		}
	pri("---------- \n");
	return 0;
}


int main(int argc ,char **argv)
{
	char *out = argv[0];

	key_t k_id = ftok("adams.c", 1);			//生成信号量的键值
	if(-1 == k_id)
		{
			perror("ftok error ");
			return -1;
		}

	int SemId = semget(k_id, 1, IPC_CREAT | 0666);	//创建信号量
	if(-1 == SemId)
		{
			perror("semget error ");
			return -1;
		}

	pri("SemId :%d \n", SemId);

	int ret = InitSem(SemId);						//初始化信号量
	if(-1 == ret) return -1;

	while(1)
		{
			ret = PFunc(SemId);
			if(-1 == ret) return -1;

			sleep(rand() % 3);
			printf("[%ld] :%s \n", time(NULL),out+2);

			ret = VFunc(SemId);
			if(-1 == ret) return -1;
		}

	ret = DelSem(SemId);					//删除信号量
	if(-1 == ret) return -1;

	pri("exit ! \n");

	return 0;
}















