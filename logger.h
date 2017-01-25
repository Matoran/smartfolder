/**
 * @author ISELI Cyril & RODRIGUES Marco
 * @brief Display formatted logs in stderr
 * @version 0.1
 * @date December 2016 and January 2017
 * @file logger.h
 *
 * Display logs with datetime, with levels: LOG, DEBUG, ERROR
 * Use format like printf and variable args
 */

#ifndef SEARCHFOLDER_LOGGER_H
#define SEARCHFOLDER_LOGGER_H

#include <stdlib.h>
#include <stdbool.h>

/**
 * if is a log
 */
#define LOG 1
/**
 * if is a debug
 */
#define DEBUG 2
/**
 * if is an error
 */
#define ERROR 4
/**
 * level
 */
#define LEVEL (LOG | ERROR)
#define LEVEL (LOG | DEBUG | ERROR)

extern void logger(const char *format, int type, bool begin, ...);

extern void savePID(const char *name, pid_t pid);

extern pid_t readPID(const char *name);

extern void savePath(const char *name, const char *realpath);

extern char *readPath(const char *name);

#endif //SEARCHFOLDER_LOGGER_H
