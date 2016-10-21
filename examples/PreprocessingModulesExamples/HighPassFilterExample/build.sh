#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Syntax: build.sh test.cpp"
else
    NAME=`echo $1 | cut -d "." -f 1`
    g++ -std=c++11 -c $NAME.cpp -I/usr/local/include/GRT
    g++ $NAME.o -o $NAME -I/usr/local/include/GRT -L/usr/local/lib -lgrt
fi

