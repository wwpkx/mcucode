/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2021-02-23 12:45:28
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-02-23 12:46:52
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc,char *argv[])
{
	int fd;
	char buf[64] = "12345";
	fd = open("/dev/hello_misc",O_RDWR);//打开设备节点
	if(fd < 0)
	{
		perror("open error \n");
		return fd;
	}
	//read(fd,buf,sizeof(buf));
	
	write(fd,buf,sizeof(buf)); //向内核层写数据
	//printf("buf is %s\n",buf);
	
	close(fd);
	return 0;
}