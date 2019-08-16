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
	int len;
	int ret;
	char buf[LEN] = {0};

	key_t k_id = ftok("xiaoming.c", 1);			//生成消息对列的键值
	if(-1 == k_id)
		{
			perror("ftok error :");
			return -1;
		}
		
	int msg_id = msgget(k_id, IPC_CREAT | IPC_EXCL |0666); //创建消息对列
	/* int msg_id = msgget(k_id, IPC_CREAT | 0666); //创建消息对列 */
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
			mb.mtype = 1;
			
			printf("小明 :");
			
			str = fgets(mb.mtext, LEN,stdin);
			if(NULL !=  str)
				{
					ret = msgsnd(msg_id, &mb, strlen(str), 0); 
					if(-1 == ret) perror("msgsnd error ");
					
					len = 0;
					memset(&mb, 0, sizeof(struct msgbuf));
					mb.mtype = 1;
					
					len = msgrcv(msg_id, &mb, LEN, 1, 0);	//msgflg为0 ,会阻塞等待
					if(len>0)
						printf("小红 :%s", mb.mtext);
				
					ret = strncmp(mb.mtext,"bye", 3);
					if(0 == ret) break;
				}
		}
		
		
		ret = msgctl(msg_id, IPC_RMID, 0);	//删除对列
		if(-1 == ret)
			perror("msgctl IPC_RMID error ");
		
		
		
	pri("exit ! \n");

	return 0;
}


















