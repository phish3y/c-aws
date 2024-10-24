#ifndef AWS_H
#define AWS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libxml/xpathInternals.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>

#define AWS_CREDS_FILE ".aws/credentials"
#define AWS_CONFIG_FILE ".aws/config"

struct awscreds {
    char *key;
    char *secret;
};

struct awsconfig {
    char *region;
};

struct s3object {
    char *key;
};

int getawscreds(struct awscreds *);
int getawsconfig(struct awsconfig *);

int getxmlbody(char *, const size_t, const char *);
//

#endif