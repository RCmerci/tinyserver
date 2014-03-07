#include "returndata.h"
#include "wrap_func.h"
//extern char basedir[128];

static int func_POST(struct httpheader *header, int acfd, char *file){
    return -1;
}
/*
static int dealwith_Host(struct httpheader *header);
static int dealwith_Connection(struct httpheader *header);
//static int func_GET(struct httpheader *header, char *file);
static int dealwith_Cache_Control(struct httpheader *header);
static int dealwith_User_Agent(struct httpheader *header);
static int dealwith_Accept_Encoding(struct httpheader *header);
static int dealwith_Accept_Language(struct httpheader *header);
*/
static char* create_http_header(int statuscode, char *method, char *lastmodtime, int content_length){
    char *p = malloc(MAXHEAD), buf[256], timebuf[64];
    time_t curtime;
    bzero(p, MAXHEAD);
    if(statuscode == 200 && strncmp(method, "GET", 3)==0){
        strcat(p, "HTTP/1.1 200 OK\r\n");
        strcat(p, "Server: - -||\r\n");
        curtime = time(0);
        strcpy(timebuf, ctime(&curtime));
        timebuf[strlen(timebuf)-1] = '\0';
        sprintf(buf, "Date: %s\r\n", timebuf);
        strcat(p, buf);
        memcpy(timebuf,lastmodtime, strlen(lastmodtime)-1);
        timebuf[strlen(lastmodtime)-1] = '\0';
        sprintf(buf, "Last-Modified: %s\r\n", timebuf);
        strcat(p, buf);
        sprintf(buf, "Content-Length: %d\r\n", content_length);
        strcat(p, buf);
        strcat(p, "Content-Type: text/html\r\n\r\n");

        return p;
    }
    else if(statuscode == 404){
        strcat(p, "HTTP/1.1 404 Not Found\r\n");
        strcat(p, "Server: - -||\r\n");
        strcat(p, "Content-Type: text/html\r\n\r\n");
        return p;
    }
    else
        return NULL;
}

static int func_GET(struct httpheader *header, char *file, int acfd){
    DIR *dir;
    char filepath[256], filename[128], *p, *httphead, filebuf[FILEBUF];
    //struct dirent *dp;
    struct stat *statp;
    int content_length, nfread;
    time_t lmtime;
    FILE *filep;

    if(strlen(header->GET)==1&&strncmp(header->GET, "/", 1)==0)
        sprintf(file, "/index");
    else
        memcpy(file, header->GET,strlen(header->GET));
    
    sprintf(filepath,"%s%s", basedir, file);
    printf("for test,filepath::%s\n",filepath);
    p = strrchr(file, '/');
    memcpy(filename, p+1, strlen(p+1));
/*
    if((dir = opendir(filepath)) == NULL) return 404;
      
    while((dp = readdir(dir)) != NULL){
        if(strncmp(dp->d_name, filename, strlen(filename))==0) break;
    }
    if(dp == NULL) return 404;
  */
    statp = malloc(sizeof(struct stat));///////////malloc
    if(stat(filepath, statp) == -1) {
        free(statp);
        return 404;
    }
    content_length = statp->st_size;
    lmtime = statp->st_mtime;
    if((filep = fopen(filepath, "r")) == NULL) {
        printf("for test,fopen break\n");
        free(statp);
        return 404;
    } 
    httphead = create_http_header(200, "GET", ctime(&lmtime), content_length);
    printf("%s...\n",httphead);
    write(acfd, httphead, strlen(httphead));
    free(httphead);
    free(statp);
        
    while((nfread = fread(filebuf, 1, FILEBUF, filep)) > 0){
        write(acfd, filebuf, nfread);
    }
    return 200;
}

int return_data(struct httpheader *header, int acfd){
    char file[256], *httphead, buffor404[128];
    int statuscode;

    getcwd(basedir, 128);

    if (header->isGET == 1)
        statuscode = func_GET(header, file, acfd);
    else 
        statuscode = func_POST(header, acfd, file);

    if(statuscode == 200) {close(acfd);return 0;}

    if(statuscode == 404){
        httphead = create_http_header(404, "GET", NULL, 0);
        write(acfd, httphead, strlen(httphead));
        sprintf(buffor404, "<h1>404</h1>");
        write(acfd, buffor404, strlen(buffor404));
        close(acfd);
        return 0;
    }
    else {
        close(acfd);
        return -1;
    }
}
