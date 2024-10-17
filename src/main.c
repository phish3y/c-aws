#include "../include/aws.h"

int main() {
    struct awscreds creds;
    if(getawscreds(&creds) < 0) {
        fprintf(stderr, "failed to get aws creds\n");
        return -1;
    }  
    fprintf(stdout, "key: %s\n", creds.key);
    fprintf(stdout, "secret: %s\n", creds.secret);

    struct awsconfig config;
    if(getawsconfig(&config) < 0) {
        fprintf(stderr, "failed to get aws config\n");
        return -1;
    }
    fprintf(stdout, "region: %s\n", config.region);

    return 0;
}