/*
 * @Author: your name
 * @Date: 2021-02-27 09:57:56
 * @LastEditTime: 2021-02-27 10:36:02
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \undefinede:\新建文件夹\驱动文件\imx6ull驱动程序配套资料\019-Linux内核定时器\34timer\helloworld.c
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
// 声明时间处理函数
static void timer_function(unsigned long data);
// 该宏会静态创建一个名叫 timer_name 内核定时器，
// 并初始化其 function, expires, name 和 base 字段。
DEFINE_TIMER(test_timer, timer_function, 0, 0);

/**
 * @description:超时处理函数 
 * @param {*}
 * @return {*}
 */
static void timer_function(unsigned long data)
{
	printk(" This is timer_function \n");
	/**
  * @description: 修改定时值，如果定时器还没有激活的话，mod_timer 函数会激活定时器
  * @param {1} *
  * @return {*}
  */
	mod_timer(&test_timer, jiffies + 1 * HZ);
}
static int hello_init(void)
{
	printk("hello world! \n");
	//初始化test_timer.expires意为超时时间
	test_timer.expires = jiffies + 1 * HZ;
	//定时器注册到内核里面，启动定时器
	add_timer(&test_timer);
	return 0;
}

static void hello_exit(void)
{
	printk("gooodbye! \n");
	// 删除一个定时器
	del_timer(&test_timer);
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
