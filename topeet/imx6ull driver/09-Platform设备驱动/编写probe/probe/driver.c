/*
 * @Author: topeet
 * @Description: 基于平台设备模型的driver.c,在probe函数中获取硬件资源后，注册一个杂项设备
 */
//初始化头文件
#include <linux/init.h>	
//最基本的文件，支持动态添加和卸载模块。		   
#include <linux/module.h>
//平台设备所需要的头文件 		  
#include <linux/platform_device.h> 
#include <linux/ioport.h>
/*注册杂项设备头文件*/
#include <linux/miscdevice.h>
//文件系统头文件，定义文件表结构（file,buffer_head,m_inode等）
#include <linux/fs.h>	
//包含了copy_to_user、copy_from_user等内核访问用户进程内存地址的函数定义。	 
#include <linux/uaccess.h>	 
//包含了ioremap、iowrite等内核访问IO内存等函数的定义。
#include <linux/io.h>		 
//寄存器地址定义
#define GPIO5_DR 0x020AC000
//存放映射完的虚拟地址的首地址
unsigned int *vir_gpio5_dr;
int ret; //返回值

struct resource *beep_mem;
struct resource *beep_mem_tmp;
/****************************************************************************************
 * @brief misc_read : 用户空间向设备写入数据时执行此函数
 * @param file  : 文件
 * @param ubuf 	: 指向用户空间数据缓冲区
 * @return 成功返回 0   	
 ****************************************************************************************/
ssize_t misc_read(struct file *file, char __user *ubuf, size_t size, loff_t *loff_t)
{
	printk("misc_read\n ");
	return 0;
}
/****************************************************************************************
 * @brief misc_write : 用户空间向驱动模块写入数据时执行此函数，对数据进行判断，控制蜂鸣器响灭
 * @param file  : 文件
 * @param ubuf 	: 指向用户空间数据缓冲区
 * @return 成功返回 0 ，失败返回 -1  	
 ****************************************************************************************/
ssize_t misc_write(struct file *file, const char __user *ubuf, size_t size, loff_t *loff_t)

{
	char kbuf[64] = {0}; //保存的是从应用层读取到的数据
	if (copy_from_user(kbuf, ubuf, size) != 0)
	{
		printk("copy_from_user error \n ");
		return -1;
	}
	printk("kbuf is %s\n ", kbuf);
	if (kbuf[0] == 1)  //如果传递进来数据为1，则打开蜂鸣器
	{
		*vir_gpio5_dr |= (1 << 1);
	}
	else if (kbuf[0] == 0)  //如果传递进来数据为0，关闭蜂鸣器

		*vir_gpio5_dr &= ~(1 << 1);

	return 0;
}
 /****************************************************************************************
 * @description:misc_release 释放 platform 设备模块的时候此函数会执行
 * @param {structinode} *inode
 * @param {structfile} *file 
 * @return {*}
 ****************************************************************************************/
int misc_release(struct inode *inode, struct file *file)
{
	printk("hello misc_relaease bye bye \n ");
	return 0;
}
/****************************************************************************************
 * @brief misc_open : 打开设备节点时执行此函数，并初始化GPIO
 * @param inode : 文件索引
 * @param file  : 文件
 * @return 成功返回 0 	    	
 ****************************************************************************************/
int misc_open(struct inode *inode, struct file *file)
{
	printk("hello misc_open\n ");
	return 0;
}

struct file_operations misc_fops = {
	.owner = THIS_MODULE,
	.open = misc_open,
	.release = misc_release,
	.read = misc_read,
	.write = misc_write,
};

struct miscdevice misc_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "hello_misc",
	.fops = &misc_fops,
};
/****************************************************************************************
 * @brief beep_probe : 与设备信息层（device.c）匹配成功后自动执行此函数，
 * @param inode : 文件索引
 * @param file  : 文件
 * @return 成功返回 0 	    	
 ****************************************************************************************/
int beep_probe(struct platform_device *pdev)
{
	printk("beep_probe\n");
	/*获取硬件资源方法一： 不推荐*/
	//printk("beep_res is %s\n",pdev->resource[0].name);
	//return 0;
	/*获取硬件资源方法二： 推荐*/
	beep_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (beep_mem == NULL)
	{
		printk("platform_get_resource is error\n");
		return -EBUSY;
	}
	printk("beep_res start is 0x%x \n", beep_mem->start);
	printk("beep_res end is 0x%x \n", beep_mem->end);

	/*beep_mem_tmp =  request_mem_region(beep_mem->start,beep_mem->end-beep_mem->start+1,"beep");
	if(beep_mem_tmp == NULL)
	{
		printk("  request_mem_region is error\n");
		goto err_region;
 err_region:
        release_mem_region(beep_mem->start,beep_mem->end-beep_mem->start+1);
	return -EBUSY;
	}*/
	/*****************************************************************/
	//映射GPIO资源
	vir_gpio5_dr = ioremap(beep_mem->start, 4);
	if (vir_gpio5_dr == NULL)
	{
		printk("GPIO5_DR ioremap is error \n");
		return EBUSY;
	}
	printk("GPIO5_DR ioremap is ok \n");

	//注册杂项设备
	ret = misc_register(&misc_dev);
	if (ret < 0)
	{
		printk("misc registe is error \n");
	}
	printk("misc registe is succeed \n");

}
int beep_remove(struct platform_device *pdev)
{
	printk("beep_remove\n");
	return 0;
}

const struct platform_device_id beep_idtable = {
	.name = "beep_test",
};
// platform 驱动结构体
struct platform_driver beep_driver = {
	.probe = beep_probe,
	.remove = beep_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name = "123"},
	.id_table = &beep_idtable

};
/**
 * @description: 设备模块加载
 * @param {*}无
 * @return {*}无
 */
static int beep_driver_init(void)
{
	int ret = 0;
	// platform驱动注册到 Linux 内核
	ret = platform_driver_register(&beep_driver);
	if (ret < 0)
	{
		printk("platform_driver_register error \n");
	}
	printk("platform_driver_register ok \n");

	return 0;
}
/**
 * @description: 设备模块注销
 * @param {*}无
 * @return {*}无
 */
static void beep_driver_exit(void)
{
	platform_driver_unregister(&beep_driver);
	misc_deregister(&misc_dev);
	iounmap(beep_mem->start);
	printk("gooodbye! \n");
}
module_init(beep_driver_init);
module_exit(beep_driver_exit);
MODULE_LICENSE("GPL");
