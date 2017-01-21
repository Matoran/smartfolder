//
// Created by matoran on 12/16/16.
//

#include <memory.h>
#include <stdio.h>
#include <zconf.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>
#include "linker.h"
#include "logger.h"
#include <unistd.h>
#include <time.h>
#include "wrappersyscall.h"

int exist(const char *linkSource, char *linkDestination){
    struct stat sb;
    char *linkname;

    if(lstat(linkDestination, &sb) == -1){
        return SYMLINK_NO_EXIST;
    }
    linkname = mallocw(sb.st_size + 1);
    readlinkw(linkDestination, linkname, sb.st_size + 1);
    linkname[sb.st_size] = '\0';

    if(strcmp(linkSource,linkname) == 0){
        free(linkname);
        return SAME_SYMLINK;
    }
    free(linkname);
    return SYMLINK_EXIST;
}

void zelda(const char *linkSource, const char *filename){
    char *linkDestination = mallocw(sizeof(char)*(strlen(linker_destination)+strlen(filename)+10));
    strcpy(linkDestination, linker_destination);
    strcat(linkDestination, "/");
    strcat(linkDestination, filename);
    int i = 0;
    bool open = true;
    size_t length = strlen(linkDestination);
    int valueExist;
    do{
        valueExist = exist(linkSource,linkDestination);
        if(valueExist == SYMLINK_EXIST) {
            i++;
            sprintf(&linkDestination[length], "%d", i);
        }else if(valueExist == SYMLINK_NO_EXIST){
            open = false;
        }else{
            return;
        }
    }while(open);

    logger("link source %s link destination %s\n", DEBUG,true, linkSource, linkDestination);

    symlinkw(linkSource, linkDestination);
    free(linkDestination);
}