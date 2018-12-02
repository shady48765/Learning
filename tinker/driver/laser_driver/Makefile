# ------------------------------------------------------- #
# Author :
# Data :
# Version
# ------------------------------------------------------- #
# get system verison
KVERS = $(shell uname -r)
# # Kernel modules
# obj-m += full_func_chrdev.o
# module install name
MODULE_NAME = laser

CROSS_COMPILE = arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc

# Kernel modules
obj-m += ${MODULE_NAME}.o

KERNEL_DIR = /home/quan/tinker/debian_kernel
# get driver floder path
CUR_DIR = $(shell pwd)

all : kernel_modules

kernel_modules:
	make -C $(KERNEL_DIR) M=$(CUR_DIR) modules ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
	# $(CC) $(APP).c -o $(APP)
	@echo "---> build finished."

app :
	$(CC) $(APP).c -o $(APP)

clean :
	make -C $(KERNEL_DIR) M=$(CUR_DIR) clean
	rm -rf $(APP)
	rm -rf .*.?.sw?p
	rm -rf .*.?.swp
	@echo "---> clean trash finished."

temp_clean:
	@echo "---> clean swap file."
	rm -rf .*.?.sw?p
	rm -rf .*.?.swp
	@echo "---> clean trash file."
	rm -rf *.o
	rm -rf *.*.o
	rm -rf .*.cmd
	rm -rf .tmp_versions
	rm -rf *.symvers
	rm -rf *.order
	rm -rf *.mod.c
	@echo "---> temp files clean finished."

install:
	@echo "---> install module $(MODULE_NAME)"
	$(shell sudo insmod $(MODULE_NAME).ko)
remove:
	@echo "---> remove module $(MODULE_NAME)"
	$(shell sudo rmmod $(MODULE_NAME).ko)
#-------------------- END LINE --------------------------
