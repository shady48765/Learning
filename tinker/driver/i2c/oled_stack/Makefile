# get system verison
KVERS = $(shell uname -r)
# get driver floder path
CURDIR := $(shell pwd)


# module install name
MODULE_NAME = driver_oled
# Kernel modules
obj-m += ${MODULE_NAME}.o
# Specify flags for the module compilation.
#EXTRA_CFLAGS=-g -O0

temp += ${MODULE_NAME}.ko 
all : kernel_modules
kernel_modules:
	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) modules
	@echo "---> build finished."
cleanall:
	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) clean
	rm -rvf .*.?.sw?p 
	rm -rvf .*.?.swp 
	@echo "---> all clean"
clean:
	@echo "---> clean trash file."
	rm -rvf .*.?.sw?p .*.?.swp *.o *.*.o .*.cmd .tmp_versions *.mod.c Module.symvers modules.order
install:
	@echo "---> install module $(temp)"
	$(shell sudo insmod $(temp))
remove:
	@echo "---> remove module $(MODULE_NAME)"
	$(shell sudo rmmod $(MODULE_NAME))