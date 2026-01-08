
/*
 * @Author: topeet
 * @Description: 应用程序与I2c通信
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/input.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
int fd;
int ret;
/**
 * @description: i2c_read_data i2c读数据
 * @param {unsignedint} slave_addr：从机设备的地址
 * @param {unsignedchar} reg_addr：寄存器的地址
 * @return {*}
 */
int i2c_read_data(unsigned int slave_addr, unsigned char reg_addr)
{
	unsigned char data;
	//定义一个要发送的数据包i2c_read_lcd
	struct i2c_rdwr_ioctl_data i2c_read_lcd;
	//定义初始化i2c_msg结构体
	//1. 设置从机的地址 
	//2. 读数据
	struct i2c_msg msg[2] = {
		[0] = {
			.addr = slave_addr,		  //设置从机地址
			.flags = 0,				  //设置为写
			.buf = &reg_addr,		  //设置寄存器的地址
			.len = sizeof(reg_addr)}, //设置寄存器的地址的长度
		[1] = {.addr = slave_addr,	  //设置从机地址
			   .flags = 1,			  //设置为读
			   .buf = &data,		  //设置寄存器的地址
			   .len = sizeof(data)},  //设置寄存器的地址
	};
	//初始化数据包的数据
	i2c_read_lcd.msgs = msg;
	//初始化数据包的个数
	i2c_read_lcd.nmsgs = 2;
	//操作读写数据包
	ret = ioctl(fd, I2C_RDWR, &i2c_read_lcd);
	if (ret < 0)
	{
		perror("ioctl error ");
		return ret;
	}
	return data;
}
int main(int argc, char *argv[])
{
	int TD_STATUS;
	//打开设备节点
	fd = open("/dev/i2c-1", O_RDWR);
	if (fd < 0)
	{
		//打开设备节点失败
		perror("open error \n");
		return fd;
	}
	while (1)
	{
		//i2C读从机地址为0x38，寄存器地址为0x02的数据
		//我们从数据手册中得知TD_STATUS的地址为0x02
		TD_STATUS = i2c_read_data(0x38, 0x02);
		// 打印TD_STATUS的值
		printf("TD_STATUS value is %d \n", TD_STATUS);
		sleep(1);
	}
	close(fd);
	return 0;
}
