//
// Created by matoran on 12/16/16.
//

#include <memory.h>
#include <stdio.h>
#include <zconf.h>
#include <stdlib.h>
#include "linker.h"

void zelda(const char *linkSource, const char *filename){

    char *linkDestination = malloc(sizeof(char)*(strlen(linker_destination)+strlen(filename)+10));
    strcpy(linkDestination, linker_destination);
    strcat(linkDestination, "/");
    strcat(linkDestination, filename);
    FILE* file = NULL;
    int i = 1;
    do{
        file = fopen(linkDestination, "r+");
        linkDestination
        i++;
    }while(file != NULL);
    sprintf(linkDestination, "%d", i);
    symlink(linkSource, linkDestination);
}