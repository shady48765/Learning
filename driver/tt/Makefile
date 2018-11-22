CROSS_COMPILE = arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc

MODULE = led_drv
APP = led_app

KERNEL_DIR = /home/quan/tinker_s/debian_kernel

CUR_DIR = $(shell pwd)


all :
	#进入内核源码，调用内核源码中Makefile, 并且给内核Makefile传递参数
	# M=$(CUR_DIR) modules 表示需要将当前目录的源码编译成模块
	# make -C $(KERNEL_DIR) M=$(CUR_DIR) modules # don't set ARCH and CROSS_COMPILE in kernel
	make -C $(KERNEL_DIR) M=$(CUR_DIR) modules ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
	$(CC) $(APP).c -o $(APP)
module : 
	make -C $(KERNEL_DIR) M=$(CUR_DIR) modules ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-

app :
	$(CC) $(APP).c -o $(APP)

clean : 
	make -C $(KERNEL_DIR) M=$(CUR_DIR) clean
	rm -rf $(APP)

install :  
	cp -raf *.ko $(APP) /opt/rootfs/drv_module

#指定将当前目录下的.o对应的.c文件编译成ko
obj-m += $(MODULE).o
