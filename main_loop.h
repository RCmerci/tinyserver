#ifndef MAIN_LOOP
#define MAIN_LOOP

#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include "wrap_func.h"

int forever_loop(int argc, char **argv, int port, int backlog);

#endif
