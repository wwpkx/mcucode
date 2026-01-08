#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc,char *argv[])
{
	int fd;
	char buf[64] = {0};
	fd = open("/dev/chrdev_test",O_RDWR);
	if(fd < 0)
	{
		perror("open error \n");
		return fd;
	}
	read(fd,buf,sizeof(buf));
	close(fd);
	return 0;
}
