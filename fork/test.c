#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define pri(fmt, ...) 	printf("["__FILE__"][%s]_%d: " ,__FUNCTION__,__LINE__ );\
						printf(fmt, ##__VA_ARGS__);

int father()
{
	pri("pid_father :%d \n", getppid());	
	pri("pid_self :%d \n", getpid());		

	int i;
	for(i=0; i<5; i++)
		{
			pri("\t%d \n", i);
			sleep(1);
		}
}

int son()
{
	pri("pid_father :%d \n", getppid());	
	pri("pid_self :%d \n", getpid());	

	int i;
	for(i=0; i<5; i++)
		{
			pri("\t%d \n", i);
			sleep(1);
		}
}


int main(int argc, char **argv)
{
	pid_t pid; 		 //进程ID
	pid = fork();	 //创建进程
	if(-1 == pid)	 //创建进程失败
		{
			perror("fork error :");
			return -1;
		}
	else if(0 == pid)	//子进程
		son();
	else				//父进程
		father();
		
		
	pri("exit ! \n");
	return 0;
}











