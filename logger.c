/**
 * \author ISELI Cyril & RODRIGUES Marco
 * \brief ADD Text
 * \version 0.1
 * \date December 2016 and January 2017
 *
 * ADD text
 */

#include "logger.h"
#include "wrappersyscall.h"

#include <stdio.h>
#include <string.h>
#include <asm/errno.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <stdarg.h>
#include <libgen.h>
#include <stdbool.h>

void writeInFile(const char *filename, const char *message){
    FILE *fp;
    fp = fopen(filename, "w");
    fprintf(fp, "%s", message);
    fprintf(fp, "\n");
    fclose(fp);
}

void logger(const char *format, int type, bool begin, ...){
    if(LEVEL & type){
        va_list args;
        va_start(args, format);
        if(begin){
            char buff[70];
            time_t t = time(NULL);
            strftime(buff, sizeof buff, "%x %T", localtime(&t));
            fprintf(stderr, "[%s] ", buff);
        }
        vfprintf(stderr, format, args);
        va_end(args);
    }
}

void savePID(const char *name, pid_t pid){
    char stringPID[12];

    char *fullName = malloc(sizeof(char)*(strlen(basename(name))+strlen("/tmp/smartfolder/")+1));
    strcpy(fullName, "/tmp/smartfolder/");
    mkdirw(fullName, S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    strcat(fullName, basename(name));
    sprintf(stringPID, "%ld", (long)pid);
    writeInFile(fullName, stringPID);
}

pid_t readPID(const char *name){
    char *fullName = mallocw(sizeof(char)*(strlen(name)+strlen("/tmp/smartfolder/")+1));
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
        logger("cannot open filename: %s, smartfolder %s doesn't exist\n", ERROR, true, fullName, name);
        exit(3);
    }
}


void savePath(const char *name, const char *realpath) {
    char *fullName = malloc(sizeof(char)*(strlen(basename(name))+strlen("/tmp/smartfolder/")+strlen("path")+1));
    strcpy(fullName, "/tmp/smartfolder/");
    strcat(fullName, basename(name));
    strcat(fullName, "path");
    writeInFile(fullName, realpath);
}

char* readPath(const char *name){
    char *fullName = mallocw(sizeof(char)*(strlen(name)+strlen("/tmp/smartfolder/")+strlen("path")+1));
    strcpy(fullName, "/tmp/smartfolder/");
    strcat(fullName, name);
    strcat(fullName, "path");
    FILE *fp;
    fp = fopen(fullName, "r");
    if(fp != NULL){
        char * line = NULL;
        size_t len = 0;
        len = getline(&line, &len, fp);
        fclose(fp);
        line[len-1] = '\0';
        remove(fullName);
        return line;
    }else{
        perror("fopen");
        exit(3);
    }
}

