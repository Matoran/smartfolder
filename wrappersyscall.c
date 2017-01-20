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

void lstatw(const char *path, struct stat *buf){
    if (lstat(path, buf) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }
}

void *mallocw(size_t size){
    void *pointer = malloc(size);
    if (pointer == NULL) {
        fprintf(stderr, "insufficient memory\n");
        exit(EXIT_FAILURE);
    }
    return pointer;
}

void readlinkw(const char *path, char *buf, size_t bufsiz){
    ssize_t r = readlink(path, buf, bufsiz);
    if (r == -1) {
        perror("error: in readlink");
        exit(EXIT_FAILURE);
    }
}
