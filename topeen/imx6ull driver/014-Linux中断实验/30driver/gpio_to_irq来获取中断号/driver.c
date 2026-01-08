/*
 * @Author:topeet
 * @Description: 使用gpio_to_irq函数来获取中断号
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_address.h>

#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/interrupt.h>
//定义结构体表示我们的节点
struct device_node *test_device_node;
struct property *test_node_property;
//要申请的中断号
int irq;
// GPIO 编号
int gpio_nu;

/**
 * @description: 中断处理函数test_key
 * @param {int} irq ：要申请的中断号
 * @param {void} *args ：
 * @return {*}IRQ_HANDLED
 */
irqreturn_t test_key(int irq, void *args)
{
    printk("test_key \n");
    return IRQ_RETVAL(IRQ_HANDLED);
}
/****************************************************************************************
 * @brief beep_probe : 与设备信息层（设备树）匹配成功后自动执行此函数，
 * @param inode : 文件索引
 * @param file  : 文件
 * @return 成功返回 0 	    	
 ****************************************************************************************/
int beep_probe(struct platform_device *pdev)
{
    int ret = 0;
    // 打印匹配成功进入probe函数
    printk("beep_probe\n");
    // of_find_node_by_path函数通过路径查找节点，/test_key是设备树下的节点路径
        test_device_node = of_find_node_by_path("/test_key");
    if (test_device_node == NULL)
    {
        //查找节点失败则打印信息
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
    irq = gpio_to_irq(gpio_nu);
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

/**
 * @description: 模块初始化函数
 * @param {*}
 * @return {*}
 */
static int beep_driver_init(void)
{
    //1.我们看驱动文件要从init函数开始看
    int ret = 0;
    //2.在init函数里面注册了platform_driver
    ret = platform_driver_register(&beep_driver);
    if (ret < 0)
    {
        printk("platform_driver_register error \n");
    }
    printk("platform_driver_register ok \n");
    return 0;
}

/**
 * @description: 模块卸载函数
 * @param {*}
 * @return {*}
 */
static void beep_driver_exit(void)
{
    free_irq(irq, NULL);
    platform_driver_unregister(&beep_driver);
    printk("gooodbye! \n");
}
module_init(beep_driver_init);
module_exit(beep_driver_exit);

MODULE_LICENSE("GPL");
