#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc,char *argv[])
{
	int fd;
	char buf[64] = {0};
	fd = open("/dev/test",O_RDWR);  //打开设备节点
	if(fd < 0)
	{
		perror("open error \n");
		return fd;
	}
	//read(fd,buf,sizeof(buf)); //从文件中读取数据放入缓冲区中
	close(fd);
	return 0;
}