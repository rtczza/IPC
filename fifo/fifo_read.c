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

// 读进程
int main(int argc, char **argv)
{
	int len;
	char buf[LEN] = {0};

	int fd = open("fifo_test",O_RDONLY);
	printf("fd = %d\n",fd);

	while(1)
		{
			len = 0;
			memset(buf, 0, LEN);
			len = read(fd, buf, LEN);
			if(len>0)
				printf("fifo_read :%s \n", buf);

			if(0 == len)
				break;
		}

	close(fd);

	pri("exit ! \n");

	return 0;
}




