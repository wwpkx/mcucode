/*
 * @Descripttion: 驱动模块传普通参数
 * @version: 
 * @Author:topeet
 * @Date: 2021-02-23 14:51:57
 */
#include <linux/init.h>   
#include <linux/module.h> 
//定义整型变量a
static int a;
//传递普通的参数a,参数权限为S_IRUSR，意为可读
module_param(a ,int,S_IRUSR);
static int hello_init(void){
    //打印变量a
	printk("a = %d \n",a);  
	printk("hello world! \n");
	return 0;
}
static void hello_exit(void){
    //打印变量a
	printk("a = %d \n",a); 
	printk("gooodbye! \n");
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");