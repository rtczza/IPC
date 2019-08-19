#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>


#define pri(fmt, ...) do { 	printf("["__FILE__"][%s]-%d: " ,__FUNCTION__,__LINE__ );\
						printf(fmt, ##__VA_ARGS__);} while (0)

#define SIZE 4096

typedef struct ShmAddr_tag
{
	int flag; //读线程是否已经读过: 1:写了,还没有读。 0:读了, 还没有写
	char buf[SIZE]; //保存进程间通讯的数据
}shmaddr;

int main()
{
	int len, ret;
	key_t k_id = ftok("shmwrite.c", 1);					//生成共享内存的键值
	if(-1 == k_id)
		{
			perror("ftok error :");
			return -1;
		}

	int ShmId = shmget(k_id, SIZE, IPC_CREAT | 0666);	//获取共享存储的ID
	if(-1 == ShmId)
		{
			perror("shmget error :");
			return -1;
		}

	pri("ShmId :%d \n", ShmId);

	shmaddr *addr;
	addr = shmat(ShmId, (void *)0, 0);					//获取共享存储段地址
	if(NULL == addr)
		{
			perror("shmat error :");
			return -1;
		}

	char *str = NULL;
	while(1)
		{
			while(addr->flag == 1)
				usleep(1000);
			
			memset(addr->buf, 0, SIZE);
			printf("shm_write: ");
			str = fgets(addr->buf, SIZE,stdin);
			
			addr->flag = 1; //修改标志位
			
			if(NULL !=  str)
				{
					ret = strncmp(addr->buf,"quit", 4);
					if(0 == ret) break;
				}
		}

	ret = shmdt(addr);		//把共享内存从进程中分离
	if(-1 == ret)
		{
			perror("shmdt error :");
			return -1;
		}

	pri("exit ! \n");
	return 0;
}












