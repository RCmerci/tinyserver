#include "main_loop.h"


static void chld_handler(int sig){
    while(waitpid(-1, NULL, WNOHANG) != -1);
}

int forever_loop(int argc, char **argv,int port, int backlog){
    int listenfd, acceptfd, addrlen, opt=1;
    struct sockaddr_in addr, ac_addr;
    pid_t pid;
    struct sigaction act;
    
    act.sa_handler = chld_handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD, &act, NULL);

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    Inet_pton(AF_INET, argv[1], &addr.sin_addr);
    Bind(listenfd, (struct sockaddr*)&addr,sizeof(addr));
    Listen(listenfd, backlog);

    while(1){
        addrlen = sizeof(ac_addr);
        acceptfd = accept(listenfd, (struct sockaddr*)&ac_addr, &addrlen);
        if(acceptfd < 0){
            if(errno == EINTR) continue;
            else{
                fprintf(stderr, "accept\n");
                exit(EXIT_FAILURE);
            }
        }
        printf("accept\n");
        if((pid = fork()) == 0){
            //child
            char fdtoclose[5], fdtoslove[5];
            printf("child\n");
            sprintf(fdtoclose, "%d", listenfd);/*exec后原来的fd还在吗？
                                                 A:还在*/
            sprintf(fdtoslove, "%d", acceptfd);
            //Close(listenfd);
            printf("%s,,,%s\n", fdtoclose,fdtoslove);
            execlp("./solve", fdtoclose, fdtoslove, NULL);///////////////////////
            //Close(acceptfd);
            //exit();
        }
        Close(acceptfd);
    }
}
