#!/bin/bash

#CROSSTOOLDIR=~/share/wangbin/
#echo ${CROSSTOOLDIR}

#if [ -f ${CROSSTOOLDIR}/ql-ol-crosstool/ql-ol-crosstool-env-init -a -x ${CROSSTOOLDIR}/ql-ol-crosstool/ql-ol-crosstool-env-init ];then
#    source ${CROSSTOOLDIR}/ql-ol-crosstool/ql-ol-crosstool-env-init
#else
#    echo "file not exisit or file can't excute"
#fi


TARGET_ARCH=x86
#TARGET_ARCH=arm

if [ ${TARGET_ARCH} == x86 ] 
then
        PLATFORM_PREFIX=
        INCLUDE=-I./include
        LDFLAGS=-Wl,-Bstatic
elif [ ${TARGET_ARCH} == arm ]
then
        PLATFORM_PREFIX=arm-oe-linux-gnueabi-
fi

CC=${PLATFORM_PREFIX}g++
#CFLAGS=-Wall -W


${CC} ${INCLUDE} TcpServer.cpp main.c Socket.cpp NetInterface.cpp SocketHelp.cpp -o server -lpthread

#${CC} send.c -o send

#${CC} recv.c -o recv
