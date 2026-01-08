#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>

static struct i2c_client *ft5x06_client;
static void ft5x06_write_reg(u8 reg_addr, u8 data, u8 len);
static int ft5x06_read_reg(u8 reg_addr);
//读寄存器函数
static int ft5x06_read_reg(u8 reg_addr)
{
	u8 data;
	struct i2c_msg msgs[] = {
		[0] = {
			.addr = ft5x06_client->addr,
			.flags = 0,
			.len = sizeof(reg_addr),
			.buf = &reg_addr,
		},

		[1] = {
			.addr = ft5x06_client->addr,
			.flags = 1,
			.len = sizeof(data),
			.buf = &data,
		},
	};
	i2c_transfer(ft5x06_client->adapter, msgs, 2);
	return data;
}
//写寄存器函数
static void ft5x06_write_reg(u8 reg_addr, u8 data, u8 len)
{
	u8 buff[256];
	struct i2c_msg msgs[] = {
		[0] = {
			.addr = ft5x06_client->addr,
			.flags = 0,
			.len = len + 1,
			.buf = buff,
		},

	};

	buff[0] = reg_addr;
	memcpy(&buff[1], &data, len);
	i2c_transfer(ft5x06_client->adapter, msgs, 1);
}
//与设备树的 compatible 匹配
static const struct of_device_id ft5x06_id[] = {
	{.compatible = "edt,edt-ft5306", 0},
	{.compatible = "edt,edt-ft5x06", 0},
	{.compatible = "edt,edt-ft5406", 0},
	{}};
// 无设备树的时候匹配 ID 表
static const struct i2c_device_id ft5x06_id_ts[] = {
	{"xxxxx", 0},
	{}};
/* i2c 驱动的 remove 函数 */
int ft5x06_remove(struct i2c_client *i2c_client)
{
	return 0;
}
/* i2c 驱动的 probe 函数 */
int ft5x06_probe(struct i2c_client *i2c_client, const struct i2c_device_id *id)
{
	int ret;
	printk("This is ft5x06_probe\n");
	//因为我们要在别的函数里面使用client，所以我们要把他复制出来
	ft5x06_client = i2c_client;
	//往地址为0x80的寄存器里面写入数据0x4b
	ft5x06_write_reg(0x80, 0x4b, 1);
	//读出0x80寄存器的值
	ret = ft5x06_read_reg(0x80);
	//打印0x80寄存器的值
	printk("ret is %#x\n", ret);
	return 0;
}

//定义一个i2c_driver的结构体
static struct i2c_driver ft5x06_driver = {

	.driver = {
		.owner = THIS_MODULE,
		.name = "ft5x06_test",
		// 采用设备树的时候驱动使用的匹配表
		.of_match_table = ft5x06_id,
	},
	.probe = ft5x06_probe,
	.remove = ft5x06_remove,
	.id_table = ft5x06_id_ts};
/* 驱动入口函数 */
static int ft5x06_driver_init(void)
{
	int ret;
	//注册 i2c_driver
	ret = i2c_add_driver(&ft5x06_driver);
	if (ret < 0)
	{
		printk(" i2c_add_driver is error \n");
		return ret;
	}
	printk("This is ft5x06_driver_init\n");
	return 0;
}
/* 驱动出口函数 */
static void ft5x06_driver_exit(void)
{
	i2c_del_driver(&ft5x06_driver);
	printk("This is ft5x06_driver_exit\n");
}

module_init(ft5x06_driver_init);
module_exit(ft5x06_driver_exit);
MODULE_LICENSE("GPL");
