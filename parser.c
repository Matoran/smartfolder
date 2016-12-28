//
// Created by matoran on 12/16/16.
//

#include "parser.h"
#include "linker.h"

static int isDirectory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

void parser(int argc, char *argv[]){
    debug("parser begin");
    if(argc < 2){
        printf("usage: ./smartfolder destination source");
    }
    if(isDirectory(argv[1])){
        linker_destination = argv[1];
    }else{
        printf("error: destination is not a directory\n");
    }

    if(isDirectory(argv[2])){
        linker_destination = argv[2];
    }else{
        printf("error: source is not a directory\n");
    }
    debug("parser end");
}