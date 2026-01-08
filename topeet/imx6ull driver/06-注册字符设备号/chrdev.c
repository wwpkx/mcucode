/*
 * @Descripttion: 动态或者静态申请字符设备号
 * @version: 
 * @Author: topeet
 */
#include <linux/init.h>         //初始化头文件
#include <linux/module.h>       //最基本的文件，支持动态添加和卸载模块。
#include <linux/fs.h>           //包含了文件操作相关struct的定义，例如大名鼎鼎的struct file_operations
#include <linux/kdev_t.h>       //

#define DEVICE_NUMBER 1         //定义次设备号的个数
#define DEVICE_SNAME "schrdev"  //定义静态注册设备的名称
#define DEVICE_ANAME "achrdev"  //定义动态注册设备的名称
#define DEVICE_MINOR_NUMBER 0   //定义次设备号的起始地址

static int major_num,minor_num; //定义主设备号和次设备号
module_param(major_num,int,S_IRUSR); //驱动模块传入普通参数major_num
module_param(minor_num ,int,S_IRUSR);//驱动模块传入普通参数minor_num

static int hello_init(void){
	dev_t dev_num;
	int ret;//函数返回值
	if(major_num)
	{	
	/*静态注册设备号*/
		printk("major_num = %d\n",major_num);//打印传入进来的主设备号
		printk("minor_num = %d\n",minor_num);//打印传入进来的次设备号
		dev_num = MKDEV(major_num,minor_num);//MKDEV将主设备号和次设备号合并为一个设备号
		ret = register_chrdev_region(dev_num, DEVICE_NUMBER,DEVICE_SNAME);//注册设备号
		if(ret<0)
		{
			printk("register_chrdev_region error\n");
		}
        //静态注册设备号成功,则打印。
		printk("register_chrdev_region ok\n");
		
	}
	else
	{
	/*动态注册设备号*/
	ret = alloc_chrdev_region(&dev_num,DEVICE_MINOR_NUMBER,1, DEVICE_ANAME);
	if(ret<0)
		{
			printk("alloc_chrdev_region error\n");
		}
        //动态注册设备号成功，则打印
		printk("alloc_chrdev_region ok\n");
		major_num =MAJOR(dev_num); //将主设备号取出来
		minor_num = MINOR(dev_num);//将次设备号取出来
		printk("major_num = %d\n",major_num);//打印传入进来的主设备号
		printk("minor_num = %d\n",minor_num);//打印传入进来的次设备号
	}
	return 0;
}

static void hello_exit(void){
	unregister_chrdev_region(MKDEV(major_num,minor_num),DEVICE_NUMBER);//注销设备号
	//printk("a = %d \n",a);
	printk("gooodbye! \n");
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");