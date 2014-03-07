#include "wrap_func.h"
#include "httpparse.h"
#include "returndata.h"
#include <strings.h>
#include <signal.h>
#define READMAX 512
void doit(int acfd);
void alarm_handler(int sig);

int main(int argc, char **argv){
    //sleep(30);
    printf("pid:%d\n",getpid());
    Close(atoi(argv[0]));
    //printf("fortest2\n");
    doit(atoi(argv[1]));
    //printf("fortest3\n");
    //Close(atoi(argv[2]));
    return 0;
}

void doit(int acfd){
    sleep(5);
    char tobeparse[2048],*p;
    int size=0, n;
    struct httpheader *header;
    bzero(tobeparse, 2048);
    //printf("fortest\n");

    signal(SIGALRM, alarm_handler);
    alarm(10);
    while(1){
        n = Read(acfd, tobeparse+size, READMAX);
        if (n == 0)break;
        size +=n;
        p = strchr(tobeparse, '\0');
        if(strncmp("\r\n\r\n", p-4, 4)==0){printf("%s\n",tobeparse); break;}
        //printf("%d\n",n);
    }
    alarm(0);
    
    //printf("%s\n",tobeparse);
    printf("out while,%d\n",getpid());
    header = httpparse(tobeparse);
    printf("out httpparse\n");
    
    return_data(header, acfd);
}

void alarm_handler(int sig){
    fprintf(stderr, "timeout\n");
    exit(EXIT_FAILURE);
}
