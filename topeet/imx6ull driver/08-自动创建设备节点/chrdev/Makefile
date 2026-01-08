obj-m += chrdev.o  #先写生成的中间文件的名字是什么，-m的意思是把我们的驱动编译成模块
KDIR:=/home/topeet/driver/imx6ull/linux-imx-rel_imx_4.1.15_2.1.0_ga/
PWD?=$(shell pwd)   #获取当前目录的变量
all:
	make -C $(KDIR) M=$(PWD) modules  #make会进入内核源码的路径，然后把当前路径下的代码编译成模块
