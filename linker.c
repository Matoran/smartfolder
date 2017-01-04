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

void zelda(const char *linkSource, const char *filename){

    char *linkDestination = malloc(sizeof(char)*(strlen(linker_destination)+strlen(filename)+10));
    strcpy(linkDestination, linker_destination);
    strcat(linkDestination, "/");
    strcat(linkDestination, filename);
    printf("destination %s\n",linker_destination);
    FILE* file = NULL;
    int i = 0;
    bool open = true;
    int length = strlen(linkDestination);
    do{
        file = fopen(linkDestination, "r+");
        if(file != NULL) {
            fclose(file);
            i++;
            sprintf(&linkDestination[length], "%d", i);
        }else{
            open = false;
        }
    }while(open);

    symlink(linkSource, linkDestination);
}