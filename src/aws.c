#include "../include/aws.h"

/**
 * getawscreds
 * 
 * @param creds: A pointer to the struct of which `key` and `secret` will be allocated to
 * @return: 0 on success, -1 on failure
 * 
 * Note: The caller must free the memory allocated for `key` and `secret`
 */
int getawscreds(struct awscreds *creds) {
    if(creds == NULL) {
        fprintf(stderr, "output struct must not be null\n");
        return -1;
    }

    const char *home = getenv("HOME");

    char credspath[1024];
    int res = snprintf(
        credspath,
        sizeof(credspath),
        "%s/%s",
        home,
        AWS_CREDS_FILE
    );
    if(res < 0) {
        fprintf(stderr, "failed to build aws credentials filepath\n");
        return -1;
    }

    FILE *credsfile = fopen(credspath, "rb");
    if (credsfile == NULL) {
        fprintf(stderr, "failed to read aws credentials file\n");
        return -1; 
    }

    fseek(credsfile, 0, SEEK_END);
    long filesize = ftell(credsfile);
    fseek(credsfile, 0, SEEK_SET);

    char buf[filesize + 1];
    size_t bytesRead = fread(buf, 1, filesize, credsfile);
    buf[bytesRead] = '\0';
    
    fclose(credsfile);

    char key[256];
    char secret[256];

    char *line = strtok((char *) buf, "\n");
    while(line != NULL) {
        if(strstr(line, "aws_access_key_id") != NULL) {
            sscanf(line, "aws_access_key_id = %s", key);
        } else if(strstr(line, "aws_secret_access_key") != NULL) {
            sscanf(line, "aws_secret_access_key = %s", secret);
        }

        line = strtok(NULL, "\n");
    }

    if(strlen(key) < 1) {
        fprintf(stderr, "faield to find key\n");
        return -1;
    }
    if(strlen(secret) < 1) {
        fprintf(stderr, "faield to find secret key\n");
        return -1;
    }

    // TODO check key/secret were found
    creds->key = strdup(key);
    creds->secret = strdup(secret);

    return 0;
}

/**
 * getawsconfig
 * 
 * @param creds: A pointer to the struct of which `region` will be allocated to
 * @return: 0 on success, -1 on failure
 * 
 * Note: The caller must free the memory allocated for `region`
 */
int getawsconfig(struct awsconfig *config) {
    if(config == NULL) {
        fprintf(stderr, "output struct must not be null\n");
        return -1;
    }

    const char *home = getenv("HOME");

    char configpath[1024];
    int res = snprintf(
        configpath,
        sizeof(configpath),
        "%s/%s",
        home,
        AWS_CONFIG_FILE
    );
    if(res < 0) {
        fprintf(stderr, "failed to build aws config filepath\n");
        return -1;
    }

    FILE *configfile = fopen(configpath, "rb");
    if (configfile == NULL) {
        fprintf(stderr, "failed to read aws config file\n");
        return -1; 
    }

    fseek(configfile, 0, SEEK_END);
    long filesize = ftell(configfile);
    fseek(configfile, 0, SEEK_SET);

    char buf[filesize + 1];
    size_t bytesRead = fread(buf, 1, filesize, configfile);
    buf[bytesRead] = '\0';
    
    fclose(configfile);

    char region[256];

    char *line = strtok((char *) buf, "\n");
    while(line != NULL) {
        if(strstr(line, "region") != NULL) {
            sscanf(line, "region = %s", region);
        } 

        line = strtok(NULL, "\n");
    }
    
    if(strlen(region) < 1) {
        fprintf(stderr, "failed to find region\n");
        return -1;
    }

    config->region = strdup(region);

    return 0;
}

int getxmlbody(char *output, const size_t len, const char *response) {
    char *chunk = strstr(response, "\r\n\r\n");
    if(chunk == NULL) {
        fprintf(stderr, "failed to find start of http body\n");
        return -1;
    }
    chunk += 4;

    size_t totalxmlsize = 0;
    while(1) {
        int contentlength = 0;
        if(sscanf(chunk, "%x\r\n", &contentlength) == EOF) {
            fprintf(stderr, "failed to get content length from http body\n");
            return -1;
        } 
        if(contentlength < 1) {
            break;
        }

        chunk = strstr(chunk, "\r\n");
        if(chunk == NULL) {
            fprintf(stderr, "failed to get chunk from http body\n");
            return -1;
        } 
        chunk += 2;
        
        if(totalxmlsize + contentlength >= len) {
            fprintf(stderr, "output buffer size too small for xml body\n");
            return -1;
        }

        strncpy(output + totalxmlsize, chunk, contentlength);
        totalxmlsize += contentlength;

        chunk = strstr(chunk, "\r\n");
        if(chunk == NULL) {
            fprintf(stderr, "failed to get chunk from http body\n");
            return -1;
        }
        chunk += 2;
    }

    if (totalxmlsize < len) {
        output[totalxmlsize] = '\0';
    } else {
        fprintf(stderr, "output buffer size too small for xml body\n");
        return -1;
    }

    return 0;
}

// TODO WIP
int getlistobjectresult(const char *xml) {
    xmlDocPtr xmldoc = xmlParseMemory(xml, strlen(xml));
    if(xmldoc == NULL) {
        fprintf(stderr, "error parsing xml string\n");
        return -1;
    }

    xmlXPathContextPtr xpathctx = xmlXPathNewContext(xmldoc);
    if(xpathctx == NULL) {
        fprintf(stderr, "error to get xpath context\n");
        return -1;
    }

    xmlXPathRegisterNs(xpathctx, (xmlChar *) "s3", (xmlChar *) "http://s3.amazonaws.com/doc/2006-03-01/"); // TODO error?

    char keys[100][256]; // TODO
    int keycount = 0;

    const char *xpath = "//s3:Key";
    xmlXPathObjectPtr xpathobj = xmlXPathEvalExpression((xmlChar *) xpath, xpathctx);
    if(xpathobj == NULL) {
        fprintf(stderr, "failed to evaluate xpath\n");
        return -1;
    } else if(!xmlXPathNodeSetIsEmpty(xpathobj->nodesetval)) {
        for(int i = 0; i < xpathobj->nodesetval->nodeNr; i++) {
            xmlNodePtr xmlnode = xpathobj->nodesetval->nodeTab[i];
            xmlChar *xmlcontent = xmlNodeGetContent(xmlnode);
            
            strncpy(keys[i], (const char *) xmlcontent, 256 - 1);
            keys[i][256 - 1] = '\0';

            keycount++;

            xmlFree(xmlcontent);
        }
    }

    for(int i = 0; i < keycount; i++) {
        printf("key %d: %s\n", i + 1, keys[i]);
    }
    
    xmlXPathFreeObject(xpathobj);
    xmlXPathFreeContext(xpathctx);
    xmlFreeDoc(xmldoc);

    return 0;
}
