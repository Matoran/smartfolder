//
// Created by matoran on 12/16/16.
//

#ifndef SEARCHFOLDER_LOGGER_H
#define SEARCHFOLDER_LOGGER_H

#include <stdlib.h>
#define LOG 1
#define DEBUG 2
#define LEVEL LOG | DEBUG

static void writeInFile(const char *filename, const char *message);
extern void logger(const char *format, int type, ...);
extern void logFile(const char *message);
extern void debug(const char *message);
void savePID(const char *name, pid_t pid);
pid_t readPID(const char *name);
void savePath(const char *name, const char *realpath);
char* readPath(const char *name);

#endif //SEARCHFOLDER_LOGGER_H
