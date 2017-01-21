//
// Created by cyril on 1/20/17.
//

#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "destroyer.h"
#include "wrappersyscall.h"

static int removeLink(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf){
    if(tflag != FTW_DP) {
        unlinkw(fpath);
    }else{
        rmdirw(fpath);
    }
    return 0;
}

void destroy(char *path){
    nftww(path, removeLink, 64, FTW_DEPTH | FTW_PHYS);
}