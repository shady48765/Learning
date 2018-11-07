#!/bin/bash
while true
do
	dmesg -T --level=err | tail -10
	echo "----------------------------------------------------"
	sleep 2s
done
