#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <time.h>

#define pri(fmt, ...) 	printf("["__FILE__"][%s]-%d: " ,__FUNCTION__,__LINE__ );\
						printf(fmt, ##__VA_ARGS__);

union semun
{
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	/* struct seminfo  *__buf; */  /* Buffer for IPC_INFO
	                                         (Linux-specific) */
};


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
	s.sem_op = -1;	//表示进程希望使用资源
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
	s.sem_op = 1;	//sem_op > 0：表示进程对资源使用完毕，交回该资源
	s.sem_flg = SEM_UNDO;

	ret = semop(SemId, &s, 1);
	if(-1 == ret)
		{
			perror("semop V error ");
			return 0;
		}
	pri("---------- \n");
	return 1;
}



int main(int argc ,char **argv)
{
	int ret;
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

	while(1)
		{
			ret = PFunc(SemId); 	//进程申请资源,计数值减一
			if(-1 == ret) return -1;

			sleep(rand() % 3);

			printf("[%ld] :%s \n", time(NULL),out+2);

			ret = VFunc(SemId);		//进程释放资源,计数值加一
			if(-1 == ret) return -1;
		}

	pri("exit ! \n");

	return 0;
}















