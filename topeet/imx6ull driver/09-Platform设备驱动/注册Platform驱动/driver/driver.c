/*
 * @Author:topeet
 * @Description: 基于平台设备模型的driver.c
 */
#include <linux/init.h>   //初始化头文件
#include <linux/module.h>  //最基本的文件，支持动态添加和卸载模块。
#include <linux/platform_device.h>//平台设备所需要的头文件
/**
 * @description: beep_probe，驱动和设备匹配成功会进入此函数
 * @param {structplatform_device} *pdev
 * @return {*}
 */
int beep_probe(struct platform_device *pdev){
	printk("beep_probe\n");
	return 0;
	
}
/**
 * @description: beep_remove，当driver和device任意一个remove的时候，就会执行这个函数
 * @param {structplatform_device} *pdev
 * @return {*}
 */
int beep_remove(struct platform_device *pdev){
	printk("beep_remove\n");
	return 0;
}
// 该设备驱动支持的设备的列表 ，他是通过这个指针去指向 platform_device_id 类型的数组
const struct platform_device_id beep_idtable = {
	.name = "beep_test", //设备名字叫“beep_test”
};

// platform 驱动结构体
struct platform_driver beep_driver = {
	.probe = beep_probe,
	.remove = beep_remove,
	.driver={
		.owner = THIS_MODULE,
		.name = "beep_test"
	},
	.id_table = &beep_idtable	
};
/**
 * @description: 设备模块加载
 * @param {*}无
 * @return {*}无
 */
static int beep_driver_init(void){
	int ret = 0;
	// platform驱动注册到 Linux 内核
	ret = platform_driver_register(&beep_driver);
	if(ret<0)
	{
		printk("platform_driver_register error \n");
	}
	printk("platform_driver_register ok \n");
	
	return 0;
}
/**
 * @description: 设备模块注销
 * @param {*}无
 * @return {*}无
 */
static void beep_driver_exit(void){
	// platform驱动卸载
	platform_driver_unregister(&beep_driver);
	printk("gooodbye! \n");
}
module_init(beep_driver_init);
module_exit(beep_driver_exit);
MODULE_LICENSE("GPL");
