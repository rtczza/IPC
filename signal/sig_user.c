#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

void func()
{
	printf(" \tSon Call This Func ! \n");
}

int father(int son_pid)
{
	int i;
	for(i=1; i<5; i++)
		{
			printf("[%d] This is Father ! \n", i);
			sleep(1);
		}

	kill(son_pid, SIGUSR1); //给子进程发信号

	sleep(2);
	printf("Father Quit ! \n");

	return 0;
}

int son()
{
	signal(SIGUSR1, func);

	pause(); //进程暂停,等待信号

	printf("Son Quit ! \n");

	return 0;
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
		father(pid);


	printf("exit ! \n");
	return 0;
}


















