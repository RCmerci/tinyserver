#ifndef RETURNDATA
#define RETURNDATA
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "httpparse.h"
#define MAXHEAD 1024
#define FILEBUF 512
char basedir[128]; 
//return value is status code (like 404)
int return_data(struct httpheader *header, int acfd);

#endif
