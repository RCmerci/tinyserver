#ifndef HTTPPARSE
#define HTTPPARSE
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
struct POSTcontent{
    char data[2048];
};

struct httpheader{
    int isGET;
    char GET[512];
    char Host[128];
    char Connection[32];
    char Cache_Control[64];
    char User_Agent[256];
    char Accept_Encoding[128];
    char Accept_Language[128];
    struct POSTcontent *postcontent;
};

struct httpheader* httpparse(char *tobeparse);
void httpheader_free(struct httpheader* header);

#endif
