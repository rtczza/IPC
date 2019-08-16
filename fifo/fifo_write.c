#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>

#define LEN 128
#define pri(fmt, ...) 	printf("["__FILE__"][%s]-%d: " ,__FUNCTION__,__LINE__ );\
						printf(fmt, ##__VA_ARGS__);


//写进程
int main(int argc, char **argv)
{
	int len;
	int ret;
	char buf[LEN] = {0};
	
	char *fifoname = "fifo_test";
	
	ret = mkfifo(fifoname, 0777);
	if(-1 == ret)
	{
		perror("mkfifo error :");
		return -1;
	}
	
	int fd = open(fifoname,O_WRONLY);
	printf("fd = %d\n",fd);

	char *str = NULL;
	while(1)
		{
			len = 0;
			memset(buf, 0, LEN);
			printf("\nfifo_write :");

			str = fgets(buf, LEN,stdin);
			if(NULL !=  str)
				{
					write(fd, buf, LEN);

					ret = strncmp(buf,"quit", 4);
					if(0 == ret)
						break;
				}
		}
		
	close(fd);

	pri("exit ! \n");

	return 0;
}

