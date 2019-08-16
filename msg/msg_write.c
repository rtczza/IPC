#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define LEN 128
#define pri(fmt, ...) 	printf("["__FILE__"][%s]-%d: " ,__FUNCTION__,__LINE__ );\
						printf(fmt, ##__VA_ARGS__);

struct msgbuf
{
	long mtype;       /* message type, must be > 0 */
	char mtext[LEN];    /* message data */
};

//写进程
int main(int argc, char **argv)
{
	int len;
	int ret;
	char buf[LEN] = {0};

	key_t k_id;
	k_id = ftok("test.c", 1);							//生成消息对列的键值
	if(-1 == k_id)
		{
			perror("ftok error :");
			return -1;
		}

	/* int msg_id = msgget(k_id, IPC_CREAT | IPC_EXCL| 0666); //创建消息对列 */
	int msg_id = msgget(k_id, IPC_CREAT | 0666); //创建消息对列
	if(-1 == msg_id)
		{
			perror("msgget error :");
			return ;
		}

	struct msgbuf mb;		//接收消息的结构体

	char *str = NULL;
	while(1)
		{
			memset(&mb, 0, sizeof(struct msgbuf));

			printf("msg_write :\n\t");

			mb.mtype = 1;
			str = fgets(mb.mtext, LEN,stdin);
			if(NULL !=  str)
				{
					/* ret = msgsnd(msg_id, &mb, LEN+sizeof(long), 0); */
					ret = msgsnd(msg_id, &mb, LEN, 0); //msgflg为0 ,会阻塞等待
					if(-1 == ret) perror("msgsnd error ");

					len = 0;
					memset(&mb, 0, sizeof(struct msgbuf));

					/* sleep(1); */


					mb.mtype = 1;
					len = msgrcv(msg_id, &mb, LEN+sizeof(long), 1, 0);
					if(len>0)
						{
							printf("\t\t\t\t\t: msg_read\n");
							printf("\t\t\t\t%s\n", mb.mtext);
						}
						
					ret = strncmp(mb.mtext,"quit", 4);
					if(0 == ret) break;
				}
		}

	pri("exit ! \n");

	return 0;
}


















