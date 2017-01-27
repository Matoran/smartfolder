/**
 * @author ISELI Cyril & RODRIGUES Marco
 * @brief Display formatted logs in stderr
 * @version 0.1
 * @date December 2016 and January 2017
 * @file logger.c
 *
 * Display logs with datetime, with levels: LOG, DEBUG, ERROR
 * Use format like printf and variable args
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
#include <stdbool.h>

/**
 * write in filename the message
 * @param filename path of the file
 * @param message a string
 */
void writeInFile(const char *filename, const char *message){
    FILE *fp;
    fp = fopen(filename, "w");
    fprintf(fp, "%s", message);
    fprintf(fp, "\n");
    fclose(fp);
}

/**
 * logger display in stderr, work like printf
 * @param format format of the display
 * @param type of the log defined in the header
 * @param begin if it's a new line
 * @param ... variable params like printf
 */
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

/**
 * save child PID to file in /tmp/smartfolder/{name of the smartfolder}
 * @param name of the smartfolder
 * @param pid of the smartfolder
 */
void savePID(const char *name, pid_t pid){
    char stringPID[12];

    char *fullName = mallocw(sizeof(char) * (strlen(basename(name)) + strlen("/tmp/smartfolder/") + 1));
    strcpy(fullName, "/tmp/smartfolder/");
    mkdirw(fullName, S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    strcat(fullName, basename(name));
    sprintf(stringPID, "%ld", (long)pid);
    writeInFile(fullName, stringPID);
}

/**
 * read PID wich have name as name
 * if the file doesn't exist the smartfolder also
 * @param name the name of the smartfolder
 * @return pid_t the pid
 */
pid_t readPID(const char *name, bool delete){
    char *fullName = mallocw(sizeof(char) * (strlen(name) + strlen("/tmp/smartfolder/") + 1));
    strcpy(fullName, "/tmp/smartfolder/");
    strcat(fullName, basename(name));
    FILE *fp;
    fp = fopen(fullName, "r");
    if(fp != NULL){
        pid_t value = -1;
        fscanf(fp, "%d", &value);
        fclose(fp);
        if(delete)
            remove(fullName);
        return value;
    }else{
        logger("cannot open filename: %s, smartfolder %s doesn't exist\n", ERROR, true, fullName, name);
        return -1;
    }
}

/**
 * save the absolute path of the smartfolder in /tmp/smartfolder/{name of the smartfolder}path
 * @param name the smartfolder name
 * @param realpath the absolute path of the smartfolder
 */
void savePath(const char *name, const char *realpath) {
    char *fullName = mallocw(sizeof(char)*(strlen(basename(name))+strlen("/tmp/smartfolder/")+strlen("path")+1));
    strcpy(fullName, "/tmp/smartfolder/");
    strcat(fullName, basename(name));
    strcat(fullName, "path");
    writeInFile(fullName, realpath);
}

/**
 * read path from /tmp/smartfolder/{name of the smartfolder}path
 * if the file doesn't exist the exit
 * @param name of the smartfolder
 * @return the path
 */
char* readPath(const char *name){
    char *fullName = mallocw(sizeof(char) * (strlen(name) + strlen("/tmp/smartfolder/") + strlen("path") + 1));
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
        line[len - 1] = '\0';
        remove(fullName);
        return line;
    }else{
        perror("fopen");
        exit(3);
    }
}
