#! /bin/bash

mkdir -p /root/HTTP-Server/bin 

cd /root/HTTP-Server/code
g++ -c main.cpp -std=c++20 -o /root/HTTP-Server/bin/main.o

cd /root/HTTP-Server/code/config
g++ -c config.cpp -std=c++20 -o /root/HTTP-Server/bin/config.o

cd /root/HTTP-Server/bin
g++ main.o config.o -o server 


