#!/bin/bash

git submodule init
git submodule update 

if [[ $1 == "gprof" ]]; then
    cmake -DCMAKE_VERBOSE_MAKEFILE=OFF -DPROFILING=ON -DCMAKE_BUILD_TYPE=Debug .
    make && time ./Tantrika
    echo "Writin to gprof file"
    gprof Tantrika ./gmon.out > ./gprof.txt
else
    cmake -DCMAKE_VERBOSE_MAKEFILE=OFF -DCMAKE_BUILD_TYPE=Release .
    make && time ./Tantrika
fi

