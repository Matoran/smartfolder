//
// Created by matoran on 12/16/16.
//

#ifndef SEARCHFOLDER_WRAPPERSYSCALL_H
#define SEARCHFOLDER_WRAPPERSYSCALL_H

#include <zconf.h>
#include <sys/stat.h>
#include <unistd.h>

extern pid_t forkw();
void statw(const char *restrict path, struct stat *restrict buf);

#endif //SEARCHFOLDER_WRAPPERSYSCALL_H
