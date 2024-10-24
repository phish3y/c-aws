#!/bin/bash

mkdir -p target
gcc -Wall -DDEBUG include/aws.h src/aws.c src/main.c -lssl -lcrypto $(xml2-config --cflags --libs) -o target/aws