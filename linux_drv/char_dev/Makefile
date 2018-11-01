KVERS = $(shell uname -r)

# Kernel modules
obj-m += test_module.o

# Specify flags for the module compilation.
#EXTRA_CFLAGS=-g -O0

all : kernel_modules

kernel_modules:
	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) modules
clean:
	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) clean
	@echo "---> cleanning swap file"
	rm -rf .*.?.sw?p
