/*
 * @Author:topeet
 * @Description:定时器按键消抖
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/interrupt.h>
#include <linux/of_irq.h>
#include <linux/timer.h>
// 声明时间处理函数
static void timer_function(unsigned long data);
// 该宏会静态创建一个名叫 timer_name 内核定时器，
// 并初始化其 function, expires, name 和 base 字段。
DEFINE_TIMER(test_timer, timer_function, 0, 0);
//定义结构体表示我们的节点
struct device_node *test_device_node;
struct property *test_node_property;
//要申请的中断号
int irq;
//GPIO 编号
int gpio_nu;
/**
 * @description:超时处理函数 
 * @param {*}
 * @return {*}
 */
static void timer_function(unsigned long data)
{
	//如果超时则打印信息
	printk(" This is timer_function \n");
	//mod_timer(&test_timer,jiffies + 1*HZ);
}

//中断处理函数
irqreturn_t test_key(int irq, void *args)
{
	printk("test_key\n");
	test_timer.expires = jiffies + msecs_to_jiffies(20);
	//定时器注册到内核里面
	add_timer(&test_timer);
	return IRQ_RETVAL(IRQ_HANDLED);
}
/**
 * @brief beep_probe : 与设备信息层（设备树）匹配成功后自动执行此函数，
 * @param inode : 文件索引
 * @param file  : 文件
 * @return 成功返回 0           
*/
int beep_probe(struct platform_device *pdev)
{
	int ret = 0;
	printk("beep_probe\n");
	//of_find_node_by_path函数通过路径查找节点，/test_key是设备树下的节点路径
	test_device_node = of_find_node_by_path("/test_key");
	if (test_device_node == NULL)
	{
		printk("of_find_node_by_path is error \n");
		return -1;
	}
	//of_get_named_gpio函数获取 GPIO 编号
	gpio_nu = of_get_named_gpio(test_device_node, "gpios", 0);
	if (gpio_nu < 0)
	{
		printk("of_get_namd_gpio is error \n");
		return -1;
	}
	// 设置GPIO为输入模式
	gpio_direction_input(gpio_nu);
	// 获取中断号
	irq = irq_of_parse_and_map(test_device_node, 0);
	printk("irq is %d \n", irq);
	/*申请中断，irq:中断号名字  
     test_key：中断处理函数
     IRQF_TRIGGER_RISING：中断标志，意为上升沿触发
     "test_key"：中断的名字
     */
	ret = request_irq(irq, test_key, IRQF_TRIGGER_RISING, "test_key", NULL);
	if (ret < 0)
	{
		printk("request_irq is error \n");
		return -1;
	}
	return 0;
}

int beep_remove(struct platform_device *pdev)
{
	printk("beep_remove\n");
	return 0;
}
const struct platform_device_id beep_idtable = {
	.name = "keys",
};
const struct of_device_id of_match_table_test[] = {
	{.compatible = "test1234"},
	{},
};
struct platform_driver beep_driver = {
	//3. 在beep_driver结构体中完成了beep_probe和beep_remove
	.probe = beep_probe,
	.remove = beep_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name = "beep_test",
		.of_match_table = of_match_table_test},
	//4 .id_table的优先级要比driver.name的优先级要高，优先与.id_table进行匹配
	.id_table = &beep_idtable};

static int beep_driver_init(void)
{
	//1.我们看驱动文件要从init函数开始看
	int ret = 0;
	//2. 在init函数里面注册了platform_driver
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
	del_timer(&test_timer);
	free_irq(irq, NULL);
	platform_driver_unregister(&beep_driver);
	printk("gooodbye! \n");
}
module_init(beep_driver_init);
module_exit(beep_driver_exit);
MODULE_LICENSE("GPL");
