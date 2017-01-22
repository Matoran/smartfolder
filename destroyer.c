//
// Created by cyril on 1/20/17.
//
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <memory.h>
#include <ctype.h>
#include "destroyer.h"
#include "wrappersyscall.h"
#include "logger.h"

int scanFilter(const struct dirent *name) {
    if (name->d_type == DT_LNK) {
        return 1;
    }
    return 0;
}

int getInt(const char *string) {
    size_t length = strlen(string);
    int i = 0;
    while (isdigit(string[length - i - 1])) {
        i++;
        if (i > 8)
            break;
    }
    if (i > 0) {
        return atoi(&string[length - i]);
    } else {
        return -1;
    }
}

static int removeLink(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf) {
    if (tflag != FTW_DP) {
        unlinkw(fpath);
    } else {
        rmdirw(fpath);
    }
    return 0;
}

void destroy(char *path) {
    nftww(path, removeLink, 64, FTW_DEPTH | FTW_PHYS);
}

static int stillAlive(const char *fpath) {
    logger("path %s\n", DEBUG, true, fpath);
    struct stat sb;
    char *linkname;
    lstatw(fpath, &sb);
    linkname = mallocw(sb.st_size + 1);
    readlink(fpath, linkname, sb.st_size + 1);
    linkname[sb.st_size] = '\0';
    logger("linkname %s\n", DEBUG, true, linkname);
    return stat(linkname, &sb);
}

void checkFiles(const char *path) {
    logger("check files begin %s\n", DEBUG, true, path);
    struct dirent **namelist;
    chdir(path);
    int n = scandir(".", &namelist, scanFilter, versionsort);
    int first = n - 1;
    int distance = 0;
    if (n < 0)
        perror("scandir");
    else {
        while (n--) {
            if (n > 0) {
                int a = getInt(namelist[n]->d_name);
                int b = getInt(namelist[n - 1]->d_name);
                distance = abs(a - b);
                if ((a == 1 && b == -1) || (b == 1 && a == -1)) {
                    distance = 1;
                }
                if (distance > 1) {
                    first = n - 1;
                }
            }
            if (stillAlive(namelist[n]->d_name)) {
                unlinkw(namelist[n]->d_name);
                if (distance == 1){
                    rename(namelist[first]->d_name, namelist[n]->d_name);
                    first--;
                }
            }
            //free(namelist[n]);
        }
        free(namelist);
    }
    logger("check files end\n", DEBUG, true);
}