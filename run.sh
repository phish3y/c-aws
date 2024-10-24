#!/bin/bash

gcc -Wall -DDEBUG include/aws.h src/aws.c src/main.c $(xml2-config --cflags --libs) -o target/aws
./target/aws
