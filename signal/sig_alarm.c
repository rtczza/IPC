#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

void func()
{
	printf("This is Time To Work ! \n");	
}

int main()
{
	signal(SIGALRM, func);		//设置接收到SIGALRM信号后,执行func函数
	
	alarm(5);					//发送SIGALRM信号
	
	int i;
	for(i=0; i<7;i++)
	{
		printf("[%d] Waiting Time To Work ! \n", i);
		sleep(1);
	}

	return 0;
}
