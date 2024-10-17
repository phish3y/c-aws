#!/bin/bash

gcc -Wall -DDEBUG include/aws.h src/aws.c src/main.c -o target/aws
./target/aws
