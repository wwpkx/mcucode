
#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>

//分配一个i2c适配器指针
struct i2c_adapter *i2c_ada;

//分配一个i2c_client指针
struct i2c_client *i2c_client;

//支持i2c的设备列表
struct i2c_board_info ft5x06_info[] = {

	//每一项都代表一个i2C设备，这句话的意思是说这个设备的名字是ft5x06_test,器件地址是0x38
	{I2C_BOARD_INFO("ft5x06_test", 0x38)},
	{}};

static int ft5x06_client_init(void)
{
	//调用i2c_get_adapter获得一个i2c总线，因为ft5x06是挂载到了i2c2上，
	// 所以这个参数是1，所以这句代码的意思是把这个触摸芯片挂载到i2c2上
	i2c_ada = i2c_get_adapter(1);
	//把i2c client和i2c器件关联起来
	i2c_client = i2c_new_device(i2c_ada, ft5x06_info);

	i2c_put_adapter(i2c_ada); //释放i2c控制器

	printk("This is ft5x06_client_init \n");
	return 0;
}

static void ft5x06_client_exit(void)
{
	i2c_unregister_device(i2c_client);
	printk("This is ft5x06_client_exit \n");
}

module_init(ft5x06_client_init);
module_exit(ft5x06_client_exit);
MODULE_LICENSE("GPL");
