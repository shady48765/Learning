#!/bin/bash

####################################################################################
# RK3288 有 9 组 GPIO bank： GPIO0，GPIO1, ..., GPIO8。
# 每组又以 A0~A7, B0~B7, C0~C7, D0~D7 作为编号区分
# 不是所有 bank 都有全部编号，例如 GPIO5 就只有 B0~B7, C0~C3 
# B0 = 0 ... B7 = 7 ... C0 = 8 ... C3 = 11...
# echo GPIO5C3 ------> PIN 171 ----------> 5 x 32 + 11 = 171 -------->  
####################################################################################


sudo echo 171 > /sys/class/gpio/export 
ls /sys/class/gpio
echo out > /sys/class/gpio/gpio171/direction 

while true
do
	echo 1 > /sys/class/gpio/gpio171/value 
	sleep 1s	
	echo 0 > /sys/class/gpio/gpio171/value 
	sleep 1s	
done
