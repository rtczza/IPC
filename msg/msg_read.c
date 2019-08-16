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


// 读进程
int main(int argc, char **argv)
{
	int len, ret;

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
			pri("msgget error ! \n");
			return ;
		}

	struct msgbuf mb;		//接收消息的结构体

	while(1)				//循环读
		{
			len = 0;
			memset(&mb, 0, sizeof(struct msgbuf));

			mb.mtype = 1;
			len = msgrcv(msg_id, &mb, LEN+sizeof(long), 1, 0);
			if(len>0)
				{
					pri("msg_read :%s \n", mb.mtext);


					/* sleep(1); */

					pri("-------- \n");

					if('?' == mb.mtext[len])
						mb.mtext[len] = '!';
					else
						memcpy(mb.mtext, "over", 5);

					ret = msgsnd(msg_id, &mb, LEN, 0);
					if(-1 == ret)perror("msgsnd error ");
					else
						pri("mb.mtext :%s \n", mb.mtext);

				}
			else if(-1 == len)
				perror("msgrcv error ");
			else if(0 == len)
				break;
		}

	pri("exit ! \n");

	return 0;
}




