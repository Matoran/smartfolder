//
// Created by matoran on 12/16/16.
//

#include "logger.h"

#include <stdio.h>
#include <string.h>
#include <asm/errno.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <stdarg.h>
#include <libgen.h>

void writeInFile(const char *filename, const char *message){
    FILE *fp;
    fp = fopen(filename, "w");
    fprintf(fp, "%s", message);
    fprintf(fp, "\n");
    fclose(fp);
}

void logger(const char *filename, const char *format, int type, ...){
    if(LEVEL & type){
        va_list args;
        va_start(args, format);
        char buff[70];
        time_t t = time(NULL);
        strftime(buff, sizeof buff, "%x %T", localtime(&t));
        fprintf(stderr, "[%s] ", buff);
        fprintf(stderr, format, args);
        fprintf(stderr, "\n");
        va_end(args);
    }
}

void logFile(const char *message){
    writeInFile("log.txt", message);
}

void debug(const char *message){
    writeInFile("debug.txt", message);
}

void savePID(const char *name, pid_t pid){
    char stringPID[12];

    char *fullName = malloc(sizeof(char)*(strlen(basename(name))+strlen("/tmp/smartfolder/")+1));
    strcpy(fullName, "/tmp/smartfolder/");
    if(mkdir(fullName, S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) == -1){
        if(errno != EEXIST){
            logFile("save pid impossible");
            exit(2);
        }
    }
    strcat(fullName, basename(name));
    sprintf(stringPID, "%ld", (long)pid);
    writeInFile(fullName, stringPID);
}

pid_t readPID(const char *name){
    char *fullName = malloc(sizeof(char)*(strlen(name)+strlen("/tmp/smartfolder/")+1));
    strcpy(fullName, "/tmp/smartfolder/");
    strcat(fullName, name);
    FILE *fp;
    fp = fopen(fullName, "r");
    if(fp != NULL){
        pid_t value = -1;
        fscanf(fp, "%d", &value);
        fclose(fp);
        remove(fullName);
        return value;
    }else{
        logFile("error: cannot open filename:");
        logFile(fullName);
        exit(3);
    }
}


