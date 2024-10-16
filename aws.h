#ifndef AWS_H
#define AWS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define info(fmt, ...) fprintf(stdout, "INFO: %s:%d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define error(fmt, ...) fprintf(stderr, "ERROR: %s:%d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#ifdef DEBUG
    #define debug(fmt, ...) fprintf(stderr, "DEBUG: %s:%d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define debug(fmt, ...)
#endif

#define AWS_CREDS_FILE ".aws/credentials"
#define AWS_CONFIG_FILE ".aws/config"

struct awscreds {
    char *key;
    char *secret;
};

struct awsconfig {
    char *region;
};

int getawscreds(struct awscreds *);
int getawsconfig(struct awsconfig *);

#endif