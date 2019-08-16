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

int main(int argc, char **argv)
{
	int len, ret;
	key_t k_id = ftok("xiaoming.c", 1);			//生成消息对列的键值
	if(-1 == k_id)
		{
			perror("ftok error :");
			return -1;
		}

	int msg_id = msgget(k_id, IPC_CREAT | 0666); //创建消息对列
	if(-1 == msg_id)
		{
			pri("msgget error ! \n");
			return ;
		}

	struct msgbuf mb;		//接收消息的结构体
	while(1)
		{
			len = 0;
			memset(&mb, 0, sizeof(struct msgbuf));
			mb.mtype = 1;

			len = msgrcv(msg_id, &mb, LEN, 1, 0);		//msgflg为0 ,会阻塞等待
			if(len>0)
				{
					printf("小明 :%s", mb.mtext);

					ret = strncmp(mb.mtext+len-2,"?", 1);
					if('?' == mb.mtext[len-2])
						mb.mtext[len-2] = '!';			//把疑问句改为感叹句

					ret = msgsnd(msg_id, &mb, LEN, 0);
					if(-1 == ret)perror("msgsnd error ");
					else
						printf("小红 :%s", mb.mtext);
					
					ret = strncmp(mb.mtext,"bye", 3);
					if(0 == ret) break;
				}

			else if(-1 == len)
				perror("msgrcv error ");
			else if(0 == len)
				break;
		}

	pri("exit ! \n");

	return 0;
}




