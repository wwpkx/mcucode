/*
 * @Descripttion: 驱动模块传递数组
 * @version: 
 * @Author: topeet

 */
#include <linux/init.h>   
#include <linux/module.h> 
//定义数组b
static int b[5];
//定义实际传入进去参数的个数
static int count;
static int a;
module_param(a ,int,S_IRUSR);
//传递数组的参数
module_param_array(b,int,&count,S_IRUSR);
static int hello_init(void){
	int i;
    //循环遍历数组b的值
	for(i = 0;i<count;i++){
    //打印数组b的值
	printk("b[%d] = %d \n",i,b[i]);
	}
    //打印传入参数的个数
	printk("count= %d \n",count);
	//printk("a = %d \n",a);
	//printk("hello world! \n");
	return 0;
}
static void hello_exit(void){
	printk("a = %d \n",a);
	printk("gooodbye! \n");
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");