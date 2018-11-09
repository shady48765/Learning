# ------------------------------------------------------- #
# Author : 
# Data :
# Version
# ------------------------------------------------------- #
KER_DIR = /home/parallels/np_k1/linux
CUR_DIR = $(shell pwd)

# Kernel modules
obj-m += platform_for_dts.o
# module install name
MODULE_NAME = platform_for_dts

all : kernel_modules

kernel_modules:
	make -C $(KER_DIR) M=$(CUR_DIR) modules ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu-
	@echo "---> build finished."
clean:
	make -C $(KER_DIR) M=$(CUR_DIR) clean
	rm -rf .*.?.sw?p 
	rm -rf .*.?.swp 
	@echo "---> clean trash finished."
delete:
	@echo "---> clean swap file."
	rm -rf .*.?.sw?p 
	rm -rf .*.?.swp
	@echo "---> clean trash file."
	rm -rf *.o
	rm -rf *.*.o
	rm -rf .*.cmd
	rm -rf .tmp_versions
	rm -rf *odul*.*
	rm -rf *.mod.c
install:
	@echo "---> install module $(temp)"
	$(shell sudo insmod $(temp))
remove:
	@echo "---> remove module $(MODULE_NAME)"
	$(shell sudo rmmod $(MODULE_NAME))


temp += ${MODULE_NAME}.ko 
