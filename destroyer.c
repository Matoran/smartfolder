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
#include "logger.h"

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

static int stillAlive(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf){
    if(tflag == FTW_SL){
        logger("stillAlive path %s\n", DEBUG, true, fpath);
        struct stat sb;
        char *linkname;
        lstatw(fpath, &sb);
        linkname = mallocw(sb.st_size + 1);
        readlink(fpath, linkname, sb.st_size + 1);
        linkname[sb.st_size] = '\0';
        logger("stillAlive linkname %s\n", DEBUG, true, linkname);
        int s = stat(linkname,&sb);
        logger("stat result %d\n", DEBUG, true, s);
        if(s == -1){
            unlinkw(fpath);
        }
    }
}

void checkFiles(const char *path){
    nftww(path, stillAlive, 64, FTW_DEPTH | FTW_PHYS);
}