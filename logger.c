//
// Created by matoran on 12/16/16.
//

#include "logger.h"

#include <stdio.h>

void writeInFile(const char *filename, const char *message){
    FILE *fp;
    fp = fopen(filename, "a");
    fprintf(fp, message);
    fprintf(fp, "\n");
    fclose(fp);
}

void log(const char *message){
    writeInFile("log.txt", message);
}

void debug(const char *message){
    writeInFile("debug.txt", message);
}

void savePID(const char *name, const char *pid){
    writeInFile(name, pid);
}


