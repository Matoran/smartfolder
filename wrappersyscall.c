#define _XOPEN_SOURCE 500
#include <time.h>
#include <sys/stat.h>
#include <zconf.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ftw.h>
#include <errno.h>
#include <signal.h>
#include "wrappersyscall.h"
#include "logger.h"


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

void symlinkw(const char *path1, const char *path2){
    int s = symlink(path1, path2);
    if(s == -1){
        perror("error: symlink");
        exit(42);
    }
}

void rmdirw(const char *path){
    int r = rmdir(path);
    if (r == -1){
        perror("fail rmdir");
        exit(42);
    }
}

void unlinkw(const char *pathname){
    int un = unlink(pathname);
    if (un == -1) {
        perror("fail unlink");
        exit(42);
    }
}

void nftww(const char *dirpath, int (*fn) (const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf),
     int nopenfd, int flags){
    if (nftw(dirpath, fn, nopenfd, flags) == -1) {
        perror("nftw error");
        exit(EXIT_FAILURE);
    }
}

void mkdirw(const char *path, mode_t mode){
    if(mkdir(path, mode) == -1){
        if(errno != EEXIST){
            perror("mkdir");
            exit(2);
        }
    }
}

void killw(pid_t pid, int sig){
    if(kill(pid, sig) == -1){
        perror("kill");
        exit(2);
    }
}
