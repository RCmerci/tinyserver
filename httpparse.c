#include "httpparse.h"

struct httpheader* httpparse(char *tobeparse){
    int size=0;
    char buf[1024];
    struct httpheader *header = malloc(sizeof(struct httpheader));

    if(strncmp("GET",tobeparse, 3) == 0){
        char str[512];
        char *p;
        header->isGET = 1;
        p = strchr(tobeparse+4,' ');
        memcpy(header->GET,tobeparse+4,p-tobeparse-4);
        printf("for test,GET::%s\n",header->GET);
    }
    if(header->isGET == 0){
        header->postcontent = malloc(sizeof(struct POSTcontent));
        //复制http正文
        //to be complete
    }
    while(1){
        size = strchr(tobeparse+size, '\n')+1-tobeparse;
        sscanf(tobeparse+size, "%[^\n]", buf);
        printf("for test,buf:%s\n",buf);
        //处理 buf...
        //char *p = strchr(buf, ':');
        if(strncmp("Host", buf, 4)==0){
            char *p1 = strchr(buf, ':');
            char *p2 = strchr(buf, '\r');
            memcpy(header->Host, p1+2, p2-p1-2);
            printf("HOST:%s\n",header->Host);
            continue;
        }
        if(strncmp("Connection", buf, 10)==0){
            char *p1 = strchr(buf, ':');
            char *p2 = strchr(buf, '\r');
            memcpy(header->Connection, p1+2, p2-p1-2);
            continue;
        }
        if(strncmp("Cache-Control", buf, 13)==0){
            char *p1 = strchr(buf, ':');
            char *p2 = strchr(buf, '\r');
            memcpy(header->Cache_Control, p1+2, p2-p1-2);
            continue;
        }
        if(strncmp("User-Agent", buf, 10)==0){
            char *p1 = strchr(buf, ':');
            char *p2 = strchr(buf, '\r');
            memcpy(header->User_Agent, p1+2, p2-p1-2);
            continue;
        }
        if(strncmp("Accept-Encoding", buf, 15)==0){
            char *p1 = strchr(buf, ':');
            char *p2 = strchr(buf, '\r');
            memcpy(header->Accept_Encoding, p1+2, p2-p1-2);
            continue;
        }
        if(strncmp("Accept-Language", buf, 15)==0){
            char *p1 = strchr(buf, ':');
            char *p2 = strchr(buf, '\r');
            memcpy(header->Accept_Language, p1+2, p2-p1-2);
            continue;
        }
        if(strncmp("\r", buf, 1)==0){
            break;
        }
    }
    return header;
}

void httpheader_free(struct httpheader* header){
   if(header->isGET == 1)
       free(header);
   else{
       free(header->postcontent);
       free(header);
   }
}
