//
// Created by matoran on 12/16/16.
//

#include "linker.h"

void zelda(const char *linkSource, const char *filename){

    char *linkDestination = strcat(linker_destination, filename);
    FILE* file = NULL;
    int i = 1;
    do{
        file = fopen(linkDestination, "r+");
        strcat(linkDestination, i);
        i++;
    }while(file != NULL);

    symlink(linkSource, linkDestination);
}