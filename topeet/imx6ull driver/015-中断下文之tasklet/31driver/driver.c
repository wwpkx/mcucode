/*
 * @Author: your topeet
 * @Description: 中断下文之tasklet，实现按键打印0-99
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
//定义结构体表示我们的节点
struct device_node *test_device_node;
//定义tasklet结构体
struct tasklet_struct key_test;
//要申请的中断号
int irq;
//GPIO 编号
int gpio_nu;

/**
 * @description: tasklet 的处理函数
 * @param {unsignedlong} data:要传递给 func 函数的参数
 * @return {*}无
 */
void test(unsigned long data)
{
	int i = data;
	printk("i is %d \n", i);
	while (i--)
		printk("test_key is %d \n", i);
}

/**
 * @description: 中断处理函数test_key
 * @param {int} irq ：要申请的中断号
 * @param {void} *args ：
 * @return {*}IRQ_HANDLED
 */
irqreturn_t test_key(int irq, void *args)
{
	printk("start\n");
	tasklet_schedule(&key_test);
	printk("end\n");
	return IRQ_RETVAL(IRQ_HANDLED);
}

/**
 * @brief beep_probe : 与设备信息层（设备树）匹配成功后自动执行此函数，
 * @param inode : 文件索引
 * @param file  : 文件
 * @return 成功返回 0           
*/
int beep_probe(struct platform_device *pdev)
{ //匹配成功以后，进入到probe函数
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
	//设置GPIO为输入模式
	gpio_direction_input(gpio_nu);
	//获取GPIO对应的中断号
	//irq = gpio_to_irq(gpio_nu);
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
	/* 初始化 tasklet */
	tasklet_init(&key_test, test, 100);
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

	free_irq(irq, NULL);
	tasklet_kill(&key_test);
	platform_driver_unregister(&beep_driver);
	printk("gooodbye! \n");
}
module_init(beep_driver_init);
module_exit(beep_driver_exit);
MODULE_LICENSE("GPL");
