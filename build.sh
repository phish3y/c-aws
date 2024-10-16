#!/bin/bash

mkdir -p target
gcc -Wall -DDEBUG aws.h aws.c main.c -lssl -lcrypto -o target/aws