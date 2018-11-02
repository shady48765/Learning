#!/bin/bash
#echo "----------------------- watch log ------------------------"

#while true 
#do
#		str=`echo $(dmesg -T --level=err)`
#		if [ ! str ]
#		then
#				echo "---> "${str}
#				str=
#				echo ":"$(str)
#		fi
#		sleep 1s
#don

while true
do
		dmesg -T --level=err | tail -5
		echo "----------------------------------------------------"
		sleep 3s
done
