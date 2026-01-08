#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc,char *argv[])
{
	int fd;
	char buf[64] = {0};
	fd = open("/dev/hello_misc",O_RDWR);
	if(fd < 0)
	{
		perror("open error \n");
		return fd;
	}
	buf[0] = atoi(argv[1]);//atoi()
	//read(fd,buf,sizeof(buf));
	write(fd,buf,sizeof(buf));
	//printf("buf is %s\n",buf);
	close(fd);
	return 0;
}
