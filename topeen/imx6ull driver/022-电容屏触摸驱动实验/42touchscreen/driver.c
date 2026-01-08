#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/interrupt.h>
#include <linux/of_irq.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/workqueue.h>

#define DEVICE_MODE 0x00
#define ID_G_MODE 0xa4

int irq;

static struct device_node *ft5x06_device_node;

static struct input_dev *ft5x06_dev;

static struct i2c_client *ft5x06_client;

static int ft5x06_read_reg(u8 reg_addr);
static void ft5x06_write_reg(u8 reg_addr, u8 data, u8 len);

static void ft5x06_func(struct work_struct *work);

DECLARE_WORK(ft5x06_work, ft5x06_func);
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
		}

	};
	buff[0] = reg_addr;
	memcpy(&buff[1], &data, len);

	i2c_transfer(ft5x06_client->adapter, msgs, 1);
}
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

static void ft5x06_func(struct work_struct *work)
{
	int TOUCH1_XH, TOUCH1_XL, x;
	int TOUCH1_YH, TOUCH1_YL, y;
	int TD_STATUS;

	//读取TOUCH1_XH寄存器的值
	TOUCH1_XH = ft5x06_read_reg(0x03);
	//读取TOUCH1_XL寄存器的值
	TOUCH1_XL = ft5x06_read_reg(0x04);
	//获取X的坐标值
	x = ((TOUCH1_XH << 8) | TOUCH1_XL) & 0x0fff;

	//读取TOUCH1_YH寄存器的值
	TOUCH1_YH = ft5x06_read_reg(0x05);
	//读取TOUCH1_YL寄存器的值
	TOUCH1_YL = ft5x06_read_reg(0x06);
	//获取Y的坐标值
	y = ((TOUCH1_YH << 8) | TOUCH1_YL) & 0x0fff;

	//读取寄存器TD_STATUS的值
	TD_STATUS = ft5x06_read_reg(0x02);

	TD_STATUS = TD_STATUS & 0xf; //获取有没有手指在屏幕上

	if (TD_STATUS == 0)
	{
		//判断有没有手指按上，如果有的话就要上报按下去的事件，没有就要上报抬手事件
		input_report_key(ft5x06_dev, BTN_TOUCH, 0);
		input_sync(ft5x06_dev);
	}
	else
	{
		input_report_key(ft5x06_dev, BTN_TOUCH, 1);
		input_report_abs(ft5x06_dev, ABS_X, x);
		input_report_abs(ft5x06_dev, ABS_Y, y);
		input_sync(ft5x06_dev);
	}
}
//中断处理函数
irqreturn_t ft5x06_handler(int irq, void *args)
{
	//printk("This is ft5x06_handler\n");
	//调度工作队列
	schedule_work(&ft5x06_work);
	return IRQ_RETVAL(IRQ_HANDLED);
}
/* i2c 驱动的 probe 函数 */
int ft5x06_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret;
	int ft5x05_irq_gpio;
	int ft5x05_reset_gpio;

	printk("This is ft5x06_probe\n");
	//因为我们要在别的函数里面用到这个client，所以我们要把他复制出来。
	ft5x06_client = client;

	//获得触摸芯片的节点
	ft5x06_device_node = of_find_node_by_path("/soc/aips-bus@02100000/i2c@021a4000/edt-ft5x06@38");
	if (ft5x06_device_node == NULL)
	{
		printk("of_find_node_by_path is error\n");
		return -1;
	}
	//打印节点的名字
	printk("ft5x06_device_node is %s\n", ft5x06_device_node->name);

	//获得中断引脚的GPIO标号
	ft5x05_irq_gpio = of_get_named_gpio(ft5x06_device_node, "irq-gpios", 0);
	if (ft5x05_irq_gpio < 0)
	{
		printk("ft5x05_irq_gpio of_get_named_gpio is error\n");
		return -2;
	}
	//获得复位引脚的GPIO标号
	ft5x05_reset_gpio = of_get_named_gpio(ft5x06_device_node, "reset-gpios", 0);
	if (ft5x05_reset_gpio < 0)
	{
		printk("ft5x05_reset_gpio of_get_named_gpio is error\n");
		return -3;
	}
	// 打印中断引脚的GPIO标号
	printk("ft5x05_irq_gpio is %d\n", ft5x05_irq_gpio);
	//打印复位引脚的GPIO标号
	printk("ft5x05_reset_gpio is %d\n", ft5x05_reset_gpio);
	//在申请中断之前将GPIO释放掉
	gpio_free(ft5x05_irq_gpio);
	//申请中断引脚的GPIO
	ret = gpio_request(ft5x05_irq_gpio, "irq_gpio");

	if (ret < 0)
	{
		printk("gpio_request is error\n");
		return -4;
	}

	gpio_free(ft5x05_reset_gpio);
	//申请复位引脚的GPIO
	ret = gpio_request(ft5x05_reset_gpio, "reset_gpio");
	if (ret < 0)
	{
		printk("ft5x05_reset_gpio is error\n");
		return -5;
	}

	//把中断的引脚设置为输入
	gpio_direction_input(ft5x05_irq_gpio);
	//设置复位引脚的方向为输出，然后输出高电平，停止复位
	gpio_direction_output(ft5x05_reset_gpio, 0);
	msleep(5);
	gpio_set_value(ft5x05_reset_gpio, 1);

	//获得中断号
	irq = gpio_to_irq(ft5x05_irq_gpio);
	//申请中断
	ret = request_irq(irq, ft5x06_handler, IRQ_TYPE_EDGE_FALLING | IRQF_ONESHOT, "ft5x06_irq", NULL);
	if (ret < 0)
	{
		printk("request_irq is error\n");
		goto error_request_irq;
	}

	//设置工作模式为正常模式
	ft5x06_write_reg(DEVICE_MODE, 0, 1);
	ft5x06_write_reg(ID_G_MODE, 1, 1);
	//申请一个 input_dev输入设备
	ft5x06_dev = input_allocate_device();
	//设置 input_dev 名字
	ft5x06_dev->name = "ft5x06_input_test";

	__set_bit(EV_KEY, ft5x06_dev->evbit);	  //支持按键事件
	__set_bit(EV_ABS, ft5x06_dev->evbit);	  //支持绝对坐标事件
	__set_bit(BTN_TOUCH, ft5x06_dev->keybit); //支持按键检测

	__set_bit(ABS_X, ft5x06_dev->absbit); //支持X坐标
	__set_bit(ABS_Y, ft5x06_dev->absbit); //支持Y坐标

	//__set_bit(ABS_PRESSURE,ft5x06_dev-> keybit);//支持压力检测

	input_set_abs_params(ft5x06_dev, ABS_X, 0, 1024, 0, 0); //设置X坐标值的范围
	input_set_abs_params(ft5x06_dev, ABS_Y, 0, 600, 0, 0);	//设置Y坐标值的范围
	//input_set_abs_params(ft5x06_dev, ABS_PRESSURE, 0, 255, 0,0);//设置压力值的范围
	//向 Linux内核注册 input_dev
	ret = input_register_device(ft5x06_dev);
	if (ret < 0)
	{
		printk("input_register_device is error\n");
		goto error_input_register;
	}

	return 0;

error_request_irq:
	free_irq(irq, NULL);

error_input_register:
	free_irq(irq, NULL);
	input_unregister_device(ft5x06_dev);
	input_free_device(ft5x06_dev);
	return ret;
}
/* i2c 驱动的 remove 函数 */
int ft5x06_remove(struct i2c_client *client)
{
	printk("This is ft5x06_remove\n");
	return 0;
}
// 无设备树的时候匹配 ID 表
static const struct i2c_device_id ft5x06_id_ts[] = {
	{
		"xxx",
		0,
	},
	{/* sentinel */}};
//与设备树的 compatible 匹配
static const struct of_device_id ft5x06_id[] = {
	{
		.compatible = "edt,edt-ft5206",
		0,
	},
	{
		.compatible = "edt,edt-ft5306",
		0,
	},
	{
		.compatible = "edt,edt-ft5406",
		0,
	},
	{/* sentinel */}};

//定义一个i2c_driver的结构体
static struct i2c_driver ft5x06_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = "ft5x06_test",
		.of_match_table = ft5x06_id,
	},
	.probe = ft5x06_probe,
	.remove = ft5x06_remove,
	.id_table = ft5x06_id_ts};
/* 驱动入口函数 */
static int ft5x06_driver_init(void)
{

	int ret = 0;
	ret = i2c_add_driver(&ft5x06_driver);
	if (ret < 0)
	{
		printk("i2c_add_driver is error\n");
		return ret;
	}
	return 0;
}
/* 驱动出口函数 */
static void ft5x06_driver_exit(void)
{
	printk("This is ft5x06_driver_exit\n");
	free_irq(irq, NULL);
	input_unregister_device(ft5x06_dev);
	input_free_device(ft5x06_dev);
	i2c_del_driver(&ft5x06_driver);
}
module_init(ft5x06_driver_init);
module_exit(ft5x06_driver_exit);
MODULE_LICENSE("GPL");
