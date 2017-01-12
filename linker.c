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

/*void link(const char* source, const char* destination){
    if(symlink(source, destination)){
        Log("symlink created");
        Log(source);
        Log(destination);
        Log("\n");
    }else{
        Log("symlink error");
        Log(source);
        Log(destination);
        Log(time()); //à charger comment récup la date gettimeofday ou

        //case l'erreur de symlink
        //impossible d'écrire
        //erreur interne etc
        Log("\n");
    }
}*/

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