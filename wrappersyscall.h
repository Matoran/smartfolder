/**
 * @author ISELI Cyril & RODRIGUES Marco
 * @date December 2016 and January 2017
 */

#ifndef SEARCHFOLDER_WRAPPERSYSCALL_H
#define SEARCHFOLDER_WRAPPERSYSCALL_H

#include <zconf.h>
#include <sys/stat.h>
#include <unistd.h>

extern pid_t forkw();

extern void statw(const char *restrict path, struct stat *restrict buf);

extern void lstatw(const char *path, struct stat *buf);

extern void *mallocw(size_t size);

extern void readlinkw(const char *path, char *buf, size_t bufsiz);

extern void symlinkw(const char *path1, const char *path2);

extern void rmdirw(const char *path);

extern void unlinkw(const char *pathname);

extern void
nftww(const char *dirpath, int (*fn)(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf),
      int nopenfd, int flags);

extern void mkdirw(const char *path, mode_t mode);

extern void killw(pid_t pid, int sig);

#endif //SEARCHFOLDER_WRAPPERSYSCALL_H
