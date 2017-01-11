//
// Created by matoran on 12/16/16.
//

#ifndef SEARCHFOLDER_LOGGER_H
#define SEARCHFOLDER_LOGGER_H

#include <stdlib.h>

static void writeInFile(const char *filename, const char *message);
extern void logFile(const char *message);
extern void debug(const char *message);
void savePID(const char *name, pid_t pid);
pid_t readPID(const char *name);

#endif //SEARCHFOLDER_LOGGER_H
