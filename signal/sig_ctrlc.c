#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

void func()
{
	printf(" \tCtrl+c is Coming ! \n");	
}

int main()
{
	signal(SIGINT, func);		//设置接收到Ctrl+C信号后,执行func函数
	
	int i=0;
	while(1)
	{
		printf("[%d] Runing ... \n", i);
		sleep(1);
		i++;
	}

	return 0;
}
