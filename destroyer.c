//
// Created by cyril on 1/20/17.
//

#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "destroyer.h"

static int removeLink(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf){
    if(tflag != FTW_DP) {
        int un = unlink(fpath);

        if (un == -1) {
            perror("fail unlink");
            exit(42);
        }
        return un;
    }else{
        int r = rmdir(fpath);

        if (r == -1){
            perror("fail rmdir");
            exit(42);
        }
        return r;
    }
}

void destroy(char *path){
    if (nftw(path, removeLink, 64, FTW_DEPTH | FTW_PHYS) == -1) {
        perror("nftw error");
        exit(EXIT_FAILURE);
    }
}