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
void lstatw(const char *path, struct stat *buf);
void *mallocw(size_t size);
void readlinkw(const char *path, char *buf, size_t bufsiz);
void symlinkw(const char *path1, const char *path2);
void rmdirw(const char *path);
void unlinkw(const char *pathname);
void nftww(const char *dirpath, int (*fn) (const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf),
           int nopenfd, int flags);
void mkdirw(const char *path, mode_t mode);
void killw(pid_t pid, int sig);

#endif //SEARCHFOLDER_WRAPPERSYSCALL_H
