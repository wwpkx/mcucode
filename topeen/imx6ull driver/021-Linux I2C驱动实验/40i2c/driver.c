/*
 * @Author: topeet
 * @Description: i2c总线实现driver驱动
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>

//与设备树的 compatible 匹配
static const struct of_device_id ft5x06_id[] = {
	{.compatible = "edt,edt-ft5306", 0},
	{.compatible = "edt,edt-ft5x06", 0},
	{.compatible = "edt,edt-ft5406", 0},
	{}};
// 无设备树的时候匹配 ID 表
static const struct i2c_device_id ft5x06_id_ts[] = {
	{"xxxxx", 0},
	{}};
/* i2c 驱动的 remove 函数 */
int ft5x06_remove(struct i2c_client *i2c_client)
{
	return 0;
}
/* i2c 驱动的 probe 函数 */
int ft5x06_probe(struct i2c_client *i2c_client, const struct i2c_device_id *id)
{
	printk("This is ft5x06_probe\n");
	return 0;
}

//定义一个i2c_driver的结构体
static struct i2c_driver ft5x06_driver = {

	.driver = {
		.owner = THIS_MODULE,
		.name = "ft5x06_test",
		// 采用设备树的时候驱动使用的匹配表
		.of_match_table = ft5x06_id,
	},
	.probe = ft5x06_probe,
	.remove = ft5x06_remove,
	.id_table = ft5x06_id_ts};
/* 驱动入口函数 */
static int ft5x06_driver_init(void)
{
	int ret;
	// 注册 i2c_driver
	ret = i2c_add_driver(&ft5x06_driver);
	if (ret < 0)
	{
		printk(" i2c_add_driver is error \n");
		return ret;
	}
	printk("This is ft5x06_driver_init\n");
	return 0;
}
/* 驱动出口函数 */
static void ft5x06_driver_exit(void)
{
	// 将前面注册的 i2c_driver 也从 Linux 内核中注销掉
	i2c_del_driver(&ft5x06_driver);
	printk("This is ft5x06_driver_exit\n");
}

module_init(ft5x06_driver_init);
module_exit(ft5x06_driver_exit);
MODULE_LICENSE("GPL");
