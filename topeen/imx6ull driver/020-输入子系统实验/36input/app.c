/*
 * @Author: topeet
 * @Description: 在Ubuntu系统读取输入事件
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
int main(int argc, char *argv[])
{
	int fd;

	struct input_event test_event;
	//打开设备节点
	fd = open("/dev/input/event1", O_RDWR);
	if (fd < 0)
	{
		//打开设备节点失败
		perror("open error \n");
		return fd;
	}
	while (1)
	{
		//循环读取设备节点
		read(fd, &test_event, sizeof(test_event));
		//如果输入事件类型为按键输入，则打印输入事件的类型和值
		if (test_event.type == EV_KEY)
		{
			printf("type is %#x \n", test_event.type);
			printf("value is %#x \n", test_event.value);
		}
	}
	close(fd);
	return 0;
}
