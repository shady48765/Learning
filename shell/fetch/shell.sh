#!/bin/bash
echo "------------- process start --------------"
if [ $# -eq 2 ]
then
    echo "ready to download code to 8.1 dir"
    echo "command is ${0} ${1} ${2}"
else
    echo "usage ./shell.sh folder_name fetch_project"
    exit
fi
PROJECT_FLODER=`echo $1`
PROJECT=`echo $2`
PATH_DIR=/home/jiangq/work/jiangq/8.1
LOG_DIR=${PATH_DIR}/fetch_log

cd ${PATH_DIR}
# if current dir equal PATH_DIR
if [ "$(pwd)" != "${PATH_DIR}" ]
then
    echo "---> change to work dir error"
    exit
else
    echo "moved in ---> " $(pwd)
fi
if [ ! -d "/${PROJECT_FLODER}" ]
then
    echo "DIR has been exist,  ready to remove"
    rm -rf ${PROJECT}
    echo "---> remove dir" ${PROJECT}
    mkdir ${PROJECT}
    mkdir ${PROJECT}/fetch_log
    if [ $? -eq 0 ]
    then
        echo "dir created"
    else
        echo "dir create falied"
        exit
    fi
else
    mkdir ${PROJECT}
    mkdir ${PROJECT}/fetch_log
    if [ $? -eq 0 ]
    then
        echo "dir created"
    else
        echo "dir creat falied..."
        exit
    fi
fi
echo "---> ready to procceed. Download " ${PROJECT} " to "${PATH_DIR}/${PROJECT_FLODER}
sleep 5s
#-------------------------------------------------------------------
cd ${PATH_DIR}/${PROJECT_FLODER}
fetch ${PROJECT} | tee ${LOG_DIR}/fetch.log
if [ $? -eq 0 ]
then 
    echo "---> fetch succeed." | tee -a ${LOG_DIR}/fetch.log
else
    echo "---> fetch failed." | tee -a ${LOG_DIR}/fetch.log
    exit
fi

cd ..
echo "go to $(pwd)" | tee -a $LOG_PATH/mt8321_for_view_creat.log

tar -cvf ./mt8321_for_view/mt8321_for_view.tar.gz mt8321_for_view | tee ./mt8321_for_view/log/tar.log

du -h mt8321_for_view.tar.gz | tee -a $LOG_PATH/tar.log
cd ./mt8321_for_view
echo  "---> tar finished. " | tee -a $LOG_PATH/tar.log
echo  "---> full operations finished. " | tee -a $LOG_PATH/mt8321_for_view_creat.log
echo "----------------- succeed ------------------"
exit
