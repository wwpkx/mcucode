/*
 * @Author: topeet
 * @Description: 蜂鸣器驱动
 */

#include <linux/init.h>			   //初始化头文件
#include <linux/module.h>		   //最基本的文件，支持动态添加和卸载模块。
#include <linux/platform_device.h> //platform平台设备相关的头文件
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/miscdevice.h> //包含了miscdevice结构的定义及相关的操作函数。
#include <linux/fs.h>		  //文件系统头文件，定义文件表结构（file,buffer_head,m_inode等）
#include <linux/uaccess.h>	  //包含了copy_to_user、copy_from_user等内核访问用户进程内存地址的函数定义。
#include <linux/io.h>		  //包含了ioremap、iowrite等内核访问IO内存等函数的定义。
#include <linux/gpio.h>
#include <linux/of_gpio.h>
int size;
int beep_gpio = 0;
int ret = 0;
u32 out_values[2] = {0};
const char *str;
struct device_node *test_device_node;
struct property *test_node_property;
unsigned int *vir_gpio_dr;

ssize_t misc_read(struct file *file, char __user *ubuf, size_t size, loff_t *loff_t)
{
	printk("misc_read\n ");
	return 0;
}
ssize_t misc_write(struct file *file, const char __user *ubuf, size_t size, loff_t *loff_t)
{
	/*应用程序传入数据到内核空间，然后控制蜂鸣器的逻辑，在此添加*/

	// kbuf保存的是从应用层读取到的数据
	char kbuf[64] = {0};
	// copy_from_user 从应用层传递数据给内核层
	if (copy_from_user(kbuf, ubuf, size) != 0)
	{
		// copy_from_user 传递失败打印
		printk("copy_from_user error \n "); 
		return -1;
	}
	//打印传递进内核的数据
	printk("kbuf is %d\n ", kbuf[0]); 
	//如果传递进的数据是1，则蜂鸣器响
	if (kbuf[0] == 1)
	{
		gpio_set_value(beep_gpio, 1);
	}
	//如果传递进的数据是0，则蜂鸣器不响
	else if (kbuf[0] == 0)
		gpio_set_value(beep_gpio, 0);
	return 0;
}

int misc_release(struct inode *inode, struct file *file)
{
	printk("hello misc_relaease bye bye \n ");
	return 0;
}
int misc_open(struct inode *inode, struct file *file)
{
	printk("hello misc_open\n ");
	return 0;
}
//文件操作集
struct file_operations misc_fops = {
	.owner = THIS_MODULE,
	.open = misc_open,
	.release = misc_release,
	.read = misc_read,
	.write = misc_write,
};
//杂项设备结构体
struct miscdevice misc_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "hello_misc",
	.fops = &misc_fops,
};
/**
 * @description: platform 驱动的 probe 函数，当驱动与设备匹配以后此函数就会执行
 * @param {*}pdev : platform 设备
 * @return {*}0，成功;其他负值,失败
 */
int beep_probe(struct platform_device *pdev)
{
	printk("beep_probe\n");
	//获得设备节点
	test_device_node = of_find_node_by_path("/test");
	if (test_device_node == NULL)
	{
		printk("of_find_node_by_path is error \n");
		return -1;
	}
	/*******我们要使用我们的GPIO就要从设备树来获取**********/

	/* 	of_get_named_gpio函数获取 GPIO 编号，
	因为 Linux 内核中关于 GPIO 的 API 函数都要使用 GPIO 编号，
	此函数会将设备树中类似<&gpio1 3 GPIO_ACTIVE_LOW>的属性信息转换为对应的 GPIO 编号 */
	beep_gpio = of_get_named_gpio(test_device_node, "beep-gpio", 0);
	if (beep_gpio < 0)
	{
		printk("of_get_named_gpio is error \n");
		return -1;
	}
	printk("beep_gpio is %d \n", beep_gpio);
	//申请一个 GPIO 管脚
	ret = gpio_request(beep_gpio, "beep");
	if (ret < 0)
	{
		printk("gpio_request is error \n");
		return -1;
	}
	//设置某个GPIO为输出，并且设置默认输出值
	gpio_direction_output(beep_gpio, 1);
	//注册杂项设备
	ret = misc_register(&misc_dev);
	if (ret < 0)
	{
		printk("misc registe is error \n");
	}
	printk("misc registe is succeed \n");
	return 0;
}

int beep_remove(struct platform_device *pdev)
{
	gpio_free(beep_gpio);
	printk("beep_remove\n");
	return 0;
}
const struct platform_device_id beep_idtable = {
	.name = "beep_test",
};
const struct of_device_id of_match_table_test[] = {
	{.compatible = "test1234"},
	{},
};
struct platform_driver beep_driver = {
	//第三步 在beep_driver结构体中完成了beep_probe和beep_remove
	.probe = beep_probe,
	.remove = beep_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name = "beep_test",
		.of_match_table = of_match_table_test 
	},
	//第四步 .id_table的优先级要比driver.name的优先级要高，优先与.id_table进行匹配
	.id_table = &beep_idtable
};

static int beep_driver_init(void)
{
	//第一步 我们看驱动文件要从init函数开始看
	int ret = 0;
	//第二步 在init函数里面注册了平台设备驱动platform_driver
	ret = platform_driver_register(&beep_driver);
	if (ret < 0)
	{
		printk("platform_driver_register error \n");
	}
	printk("platform_driver_register ok \n");

	return 0;
}
static void beep_driver_exit(void)
{
	printk("gooodbye! \n");
	misc_deregister(&misc_dev);
	platform_driver_unregister(&beep_driver);
}
module_init(beep_driver_init);
module_exit(beep_driver_exit);

MODULE_LICENSE("GPL");
