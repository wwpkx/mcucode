/*
 * @Author: your name
 * @Date: 2021-02-25 10:39:09
 * @LastEditTime: 2021-02-25 12:57:47
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \undefinede:\新建文件夹\驱动文件\imx6ull驱动程序配套资料\010-设备树常用OF函数\24driver\driver.c
 */
/*
 * @Author:topeet
 * @Description: of函数获取status属性
 */
#include <linux/init.h>   	//初始化头文件
#include <linux/module.h>   //最基本的文件，支持动态添加和卸载模块。
#include <linux/of.h>		//添加头文件
int size;					//定义长度
u32 out_values[2] = {0};
const char *str;
struct device_node *test_device_node; //定义结构体表示我们的节点
struct property *test_node_property;  //定义结构体表示我们的节点属性
static int hello_init(void)
{
	int ret;
	printk("hello world! \n");
	/**********添加我们要查找的节点的代码*******************************/
	// of_find_node_by_path函数通过路径查找节点
	test_device_node = of_find_node_by_path("/test");
	if (test_device_node == NULL)
	{
		//判断是否查找节点成功
		printk("of_find_node_by_path is error \n");
		return -1;
	}
	//打印节点的名字
	printk("test_device_node name is %s\n", test_device_node->name);
	/**********获取compatible属性内容的代码****************************/
	// of_find_property函数查找节点属性
	test_node_property = of_find_property(test_device_node, "compatible", &size);
	if (test_node_property == NULL)
	{
		//判断是否查找到节点属性内容
		printk("test_node_property is error \n");
		return -1;
	}
	//打印属性compatible的名字
	printk("test_node_property name is %s\n", test_node_property->name);
	//打印属性compatible的值
	printk("test_node_property value is %s\n", (char *)test_node_property->value);
	/**********获取reg属性内容的代码**********************************/
	ret = of_property_read_u32_array(test_device_node, "reg", out_values, 2);
	if (ret < 0)
	{
		//打印获取失败
		printk("of_property_read_u32_array is error  \n"); 
		return -1;
	}
	printk("out_values[0] is 0x%8x\n", out_values[0]);
	printk("out_values[1] is 0x%8x\n", out_values[1]);
	/**********获取status属性内容的代码*********************************/
	// of_property_read_string读取字符串属性
	ret = of_property_read_string(test_device_node, "status", &str); 
	if (ret < 0)
	{
		//打印获取失败
		printk("of_property_read_string is error  \n"); 
		return -1;
	}
	//打印status属性
	printk("status is %s \n", str);
	return 0;
}
static void hello_exit(void)
{
	printk("gooodbye! \n");
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");