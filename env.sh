#!/bin/bash

UNAME_V=`cat /etc/issue | head -n 1`

let INDEX=`expr "$UNAME_V" : ".*Debian.*"`
if [ $INDEX -gt 0 ]; then
    OS_VERSION="ubuntu16.04"
fi

let INDEX=`expr "$UNAME_V" : ".*Ubuntu.*"`
if [ $INDEX -gt 0 ]; then
    OS_VERSION="ubuntu16.04"
fi

if [ -f /etc/centos-release ]; then
    OS_VERSION="centos7.4"
    INSTALL_METHOD="yum install -y *"
fi

export ROOT_HOME=$PWD
export NEUWARE_HOME=/usr/local/neuware
export TF_SET_ANDROID_WORKSPACE=0

export X86_LIB_PATH=${NEUWARE_HOME}/lib64

export ARM32_COMPILER_PATH=~/Cambricon-Test/style_transfer/for_AT520/toolchain/arm-linux-gnueabihf-4.8.3-201404/bin/arm-linux-gnueabihf-g++
export ARM32_INCLUDE_PATH=~/Cambricon-Test/style_transfer/for_AT520/arm32_linux_lib
export ARM32_LIB_PATH=~/Cambricon-Test/style_transfer/for_AT520/lib32

export PATH=$PATH:$NEUWARE_HOME/bin
export LD_LIBRARY_PATH=$NEUWARE_HOME/lib64:$LD_LIBRARY_PATH
export GLOG_alsologtostderr=true
# Set log level which is output to stderr, 0: INFO/WARNING/ERROR/FATAL, 1: WARNING/ERROR/FATAL, 2: ERROR/FATAL, 3: FATAL,
export GLOG_minloglevel=0

source ~/.bashrc
