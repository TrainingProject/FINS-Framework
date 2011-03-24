#!/bin/bash
clear
gcc -fPIC -c -o socket_interceptor.o socket_interceptor.c
gcc -shared -o socket_interceptor.so socket_interceptor.o -ldl -lpcap -lpthread
LD_PRELOAD="./socket_interceptor.so" ./server

