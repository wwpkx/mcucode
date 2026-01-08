/*
 * @Author: topeet
 * @Description: 基于平台设备模型的device.c
 */
#include <linux/init.h>     //初始化头文件
#include <linux/module.h>   //最基本的文件，支持动态添加和卸载模块。
#include <linux/platform_device.h> //平台设备所需要的头文件
/**
 * @description: 释放 flatform 设备模块的时候此函数会执行
 * @param {structdevice} *dev:要释放的设备
 * @return {*}
 */
void beep_release(struct device *dev)
{
	printk("beep_release \n");
}
// 设备资源信息，也就是蜂鸣器所使用的所有寄存器
struct resource beep_res[] = {
	[0] = {
		.start = 0x020AC000,
		.end = 0x020AC003,
		.flags = IORESOURCE_MEM,
		.name = "GPIO5_DR",
	}

};
// platform 设备结构体
struct platform_device beep_device = {
	.name = "beep_test",
	.id = -1,
	.resource = beep_res,
	.num_resources = ARRAY_SIZE(beep_res),
	.dev = {
		.release = beep_release}};
/**
 * @description:  设备模块加载
 * @param {*}无
 * @return {*}无
 */
static int device_init(void)
{
	// 设备信息注册到 Linux 内核
	platform_device_register(&beep_device);
	printk("platform_device_register ok \n");
	return 0;
}
/**
 * @description: 设备模块注销
 * @param {*}无
 * @return {*}无
 */
static void device_exit(void)
{
	// 设备信息卸载
	platform_device_unregister(&beep_device);
	printk("gooodbye! \n");
}
module_init(device_init);
module_exit(device_exit);
MODULE_LICENSE("GPL");