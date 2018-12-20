#!/bin/bash
echo "------ clean cmake trash start ---------"
sleep 2s

rm -rvf cmake_install.cmake CMakeCache.txt
rm -rvf CMakeFiles/ -rf


echo "------ clean cmake trash finished ---------"
