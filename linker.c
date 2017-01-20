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

bool isExist(const char *linkSource, char *linkDestination){
    struct stat sb;
    char *linkname;

    lstatw(linkDestination, &sb);
    linkname = mallocw(sb.st_size + 1);
    readlinkw(linkDestination, linkname, sb.st_size + 1);
    linkname[sb.st_size] = '\0';

    if(strcmp(linkSource,linkname) == 0){
        free(linkname);
        return true;
    }
    free(linkname);
    return false;
}

void zelda(const char *linkSource, const char *filename){
    char *linkDestination = malloc(sizeof(char)*(strlen(linker_destination)+strlen(filename)+10));
    strcpy(linkDestination, linker_destination);
    strcat(linkDestination, "/");
    strcat(linkDestination, filename);
    printf("destination %s\n",linker_destination);
    FILE* file = NULL;
    int i = 0;
    bool open = true;
    size_t length = strlen(linkDestination);
    do{
        file = fopen(linkDestination, "r+");
        if(file != NULL) {
            fclose(file);
            if(isExist(linkSource,linkDestination)){
                return;
            }else{
                printf("WTF %s %s", linkSource, linkDestination);
            }
            i++;
            sprintf(&linkDestination[length], "%d", i);
        }else{
            open = false;
        }
    }while(open);

    symlink(linkSource, linkDestination);
    free(linkDestination);
}