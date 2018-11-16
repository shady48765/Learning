# ------------------------------------------------------- #
# Author : 
# Data :
# Version
# ------------------------------------------------------- #
KER_DIR = /home/parallels/np_k1/linux
CUR_DIR = $(shell pwd)

# Kernel modules
obj-m += usr_irq_driver.o
# module install name
MODULE_NAME = usr_irq_driver

all : kernel_modules

kernel_modules:
	make -C $(KER_DIR) M=$(CUR_DIR) modules ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu-
	@echo "---> build finished."
clean_all:
	make -C $(KER_DIR) M=$(CUR_DIR) clean
	rm -rf .*.?.sw?p 
	rm -rf .*.?.swp 
	@echo "---> clean trash finished."
clean:
	@echo "---> clean swap file."
	rm -rf .*.?.sw?p 
	rm -rf .*.?.swp
	@echo "---> clean trash file."
	rm -rf *.o
	rm -rf *.*.o
	rm -rf .*.cmd
	rm -rf .tmp_versions
install:
	@echo "---> install module $(temp)"
	$(shell sudo insmod $(temp))
remove:
	@echo "---> remove module $(MODULE_NAME)"
	$(shell sudo rmmod $(MODULE_NAME))


temp += ${MODULE_NAME}.ko 
