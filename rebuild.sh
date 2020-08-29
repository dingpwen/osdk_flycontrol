#!/bin/bash

build_dir="build"
if [ $1 == "clean" ];then
    rm -rvf $build_dir
fi
if [ ! -d $build_dir ];then
    mkdir -p $build_dir
fi
cd $build_dir
cmake .. && make 2>&1|tee log.txt
if [ ! -f "bin/UserConfig.txt" ];then
cp ../linux/common/UserConfig.txt bin/UserConfig.txt
fi
