/*
 * @Descripttion: 在上一章节实现了最简单杂项设备的编写，本代码再其基础上验证内核层与应用层数据交互
 */

#include <linux/init.h>   //初始化头文件
#include <linux/module.h> //最基本的文件，支持动态添加和卸载模块。
#include <linux/miscdevice.h>/*注册杂项设备头文件*/
#include <linux/uaccess.h>
#include <linux/fs.h>
/**
 * @name: misc_read
 * @test: 从设备中读取数据，当用户层调用函数read时，对应的，内核驱动就会调用这个函数。
 * @msg: 
 * @param {structfile} *file file结构体
 * @param {char__user} *ubuf 这是对应用户层的read函数的第二个参数void *buf
 * @param {size_t} size 对应应用层的read函数的第三个参数
 * @param {loff_t} *loff_t 这是用于存放文件的偏移量的，回想一下系统编程时，读写文件的操作都会使偏移量往后移。
 * @return {*} 当返回正数时，内核会把值传给应用程序的返回值。一般的，调用成功会返回成功读取的字节数。
如果返回负数，内核就会认为这是错误，应用程序返回-1
 */
ssize_t misc_read (struct file *file, char __user *ubuf, size_t size, loff_t *loff_t) 
{
	printk("misc_read\n ");
	return 0;

}
/**
 * @name: misc_write
 * @test: 往设备写入数据，当用户层调用函数write时，对应的，内核驱动就会调用这个函数。
 * @msg: 
 * @param {structfile} * filefile结构体
 * @param {constchar__user} *ubuf 这是对应用户层的write函数的第二个参数const void *buf
 * @param {size_t} size 对应用户层的write函数的第三个参数count。
 * @param {loff_t} *loff_t 这是用于存放文件的偏移量的，回想一下系统编程时，读写文件的操作都会使偏移量往后移。
 * @return {*} 当返回正数时，内核会把值传给应用程序的返回值。一般的，调用成功会返回成功读取的字节数。
 如果返回负数，内核就会认为这是错误，应用程序返回-1。
 */
ssize_t misc_write (struct file *file, const char __user *ubuf, size_t size, loff_t *loff_t)
{
   printk("misc_write\n ");
	return 0;

}

/**
 * @name: misc_release
 * @test: 当设备文件被关闭时内核会调用这个操作，当然这也可以不实现，函数默认为NULL。关闭设备永远成功。
 * @msg: 
 * @param {structinode} *inode 设备节点
 * @param {structfile} *file filefile结构体
 * @return {0}
 */
int misc_release(struct inode *inode,struct file *file)
{  
	printk("hello misc_relaease bye bye \n ");
	return 0;
}

/**
 * @name: misc_open
 * @test: 在操作设备前必须先调用open函数打开文件，可以干一些需要的初始化操作。
 * @msg: 
 * @param {structinode} *inode 设备节点
 * @param {structfile} *file filefile结构体
 * @return {0}
 */
int misc_open(struct inode *inode,struct file *file){
	printk("hello misc_open\n ");
	return 0;
}
//文件操作集
struct file_operations misc_fops=
{
	.owner = THIS_MODULE,
	.open = misc_open,
	.release = misc_release,
	.read = misc_read,
	.write = misc_write,
};
//miscdevice结构体
struct miscdevice misc_dev = 
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = "hello_misc",
	.fops = &misc_fops,
};

static int misc_init(void)
{
	int ret;
	ret = misc_register(&misc_dev);//注册杂项设备
	if(ret<0)
	{
		printk("misc registe is error \n");
	}
	printk("misc registe is succeed \n");
	return 0;
}
static void misc_exit(void)
{
	misc_deregister(&misc_dev); //卸载杂项设备
	printk(" misc gooodbye! \n");
}
module_init(misc_init);
module_exit(misc_exit);
MODULE_LICENSE("GPL");