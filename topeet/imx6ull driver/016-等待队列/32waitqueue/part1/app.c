#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc,char *argv[])
{
	int fd;
	int value;
	 //打开设备节点
	fd = open("/dev/test_wq",O_RDWR);
	if(fd < 0)
	{
		//打开设备节点失败
		perror("open error \n"); 
		return fd;
	}
	while(1)
	{
	read(fd,&value,sizeof(value));		
	//printf("value is %d \n",value); 
	}
	close(fd);
	return 0;
}

