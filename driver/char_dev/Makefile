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
obj-m += easy_chrdev.o
# module install name
MODULE_NAME = easy_chrdev

# Specify flags for the module compilation.
#EXTRA_CFLAGS=-g -O0

temp += ${MODULE_NAME}.ko 
all : kernel_modules

kernel_modules:
	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) modules
	@echo "---> build finished."
clean:
	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) clean
	@echo "---> clean swap file."
	rm -rf .*.?.sw?p 
	rm -rf .*.?.swp 
install:
	@echo "---> install module $(temp)"
	$(shell insmod $(temp))
remove:
	@echo "---> remove module $(MODULE_NAME)"
	$(shell rmmod $(MODULE_NAME))
	
