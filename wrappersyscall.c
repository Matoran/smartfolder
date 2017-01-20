#include <time.h>
#include <sys/stat.h>
#include <zconf.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "wrappersyscall.h"


pid_t forkw() {
    int val;
    if ((val = fork()) < 0) {
        perror("fork");
        exit(1);
    }
    return val;
}

void statw(const char *restrict path, struct stat *restrict buf){
    if(stat(path, buf) < 0){
        printf("error stat file");
        exit(1);
    }
}
