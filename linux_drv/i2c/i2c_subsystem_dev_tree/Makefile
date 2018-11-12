# ------------------------------------------------------- #
# Author : 
# Data :
# Version
# ------------------------------------------------------- #
# get system verison
KVERS = $(shell uname -r)
# get driver floder path
CURDIR := $(shell pwd)

# Kernel modules
obj-m += i2c_subsystem_orignal.o
# module install name
MODULE_NAME = i2c_subsystem_orignal

# Specify flags for the module compilation.
#EXTRA_CFLAGS=-g -O0

temp += ${MODULE_NAME}.ko 
all : kernel_modules

kernel_modules:
	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) modules
	@echo "---> build finished."
clean:
	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) clean
	rm -rf .*.?.sw?p 
	rm -rf .*.?.swp 
	@echo "---> clean trash finished."
clean_tansh:
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
	
