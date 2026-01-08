/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2021-02-23 12:36:04
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-02-23 12:36:54
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc,char *argv[])
{
	int fd;
	char buf[64] = {0};
	fd = open("/dev/hello_misc",O_RDWR);//打开设备节点
	if(fd < 0)
	{
		perror("open error \n");
		return fd;
	}
	read(fd,buf,sizeof(buf));//读内核层数据
	printf("buf is %s\n",buf);
	//write(fd,buf,sizeof(buf));
	close(fd);
	return 0;
}