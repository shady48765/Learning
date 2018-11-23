#!/bin/bash
echo "------------ ready to build tinker board kernel -----------------" | tee build_kernel.log
DIR=$(shell pwd)
echo "------> current PATH = " $(DIR) | tee -a build_kernel.log
echo "------> move in ./devian_kernel" | tee -a build_kernel.log
cd debian_kernel
echo "------> start to build at 5 second" |  tee -a ../build_kernel.log
sleep 5s

echo "------> distclean" |  tee -a ../build_kernel.log
make distclean

echo "------> make miniarm-rk3288_defconfig ARCH=arm" |  tee -a ../build_kernel.log
make miniarm-rk3288_defconfig ARCH=arm |  tee -a ../build_kernel.log
 
echo "------> make zImage ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-" |  tee -a ../build_kernel.log
make zImage ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- | tee -a ../build_kernel.log


echo "------> make modules ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-  CFLAGS_MODULE=-Wno-misleading-indentation" |  tee -a ../build_kernel.log
make modules ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- CFLAGS_MODULE=-Wno-misleading-indentation |tee -a ../build_kernel.log

echo "------> make rk3288-miniarm.dtb ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-" |  tee -a ../build_kernel.log
make rk3288-miniarm.dtb ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- | tee -a ../build_kernel.log

echo "------> make dtbs ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-" |  tee -a ../build_kernel.log
make dtbs ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- | tee -a ../build_kernel.log

