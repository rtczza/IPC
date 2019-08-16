#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>

#define LEN 128

#define pri(fmt, ...) 	printf("["__FILE__"][%s]-%d: " ,__FUNCTION__,__LINE__ );\
						printf(fmt, ##__VA_ARGS__);

/* pipe 无名管道  */

//写进程
int pipe_write(int *fd)
{
	close(fd[0]); //关闭读

	int len;
	int ret;
	char buf[LEN] = {0};

	char *str = NULL;

	while(1)
		{
			len = 0;
			memset(buf, 0, LEN);
			printf("\npipe_write :");

			str = fgets(buf, LEN,stdin);
			if(NULL !=  str)
				{
					write(fd[1], buf, LEN);

					ret = strncmp(buf,"quit", 4);
					if(0 == ret)
						break;
				}
		}
	close(fd[1]);

	pri("exit ! \n");

}

//读进程
int pipe_read(int *fd)
{
	close(fd[1]); //关闭写

	int len;
	int ret;
	char buf[LEN] = {0};

	while(1)
		{
			len = 0;
			memset(buf, 0, LEN);
			len = read(fd[0], buf, LEN);
			if(len>0)
				printf("pipe_read :%s \n", buf);

			if(0 == len)
				break;
		}

	close(fd[0]);

	pri("exit ! \n");
}

int main(int argc, char **argv)
{
	int pipe_fd[2];
	pipe(pipe_fd);	 //创建管道

	pid_t pid; 		 //进程ID
	pid = fork();	 //创建进程
	if(-1 == pid)	 //创建进程失败
		{
			perror("fork error :");
			return -1;
		}
	else if(0 == pid)	//子进程
		pipe_read(pipe_fd);
	else				//父进程
		pipe_write(pipe_fd);


	pri("exit ! \n");
	return 0;
}















