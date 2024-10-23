#ifndef AWS_H
#define AWS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int getxmlbody(char *, const size_t, const char *);

#endif