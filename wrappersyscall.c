/**
 * @author ISELI Cyril & RODRIGUES Marco
 * @brief Wrapper of all system functions that can fail
 * @version 0.1
 * @date December 2016 and January 2017
 * @file wrappersyscall.c
 *
 * Wrap every syscall that can fail, in general call perror and exit on error
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <ftw.h>
#include <string.h>
#include "wrappersyscall.h"

/**
 * fork()  creates  a new process by duplicating the calling process.  The
 * new process, referred to as the child, is an  exact  duplicate  of  the
 * calling  process,  referred  to as the parent
 * if fork fail then call perror and exit
 * @return child pid to parent, 0 to the child
 */
pid_t forkw() {
    int val;
    if ((val = fork()) < 0) {
        perror("fork");
        exit(1);
    }
    return val;
}

/**
 * These  functions return information about a file, in the buffer pointed to by stat.
 * if stat fail then call perror and exit
 * @param path path file to stat
 * @param buf buffer to store result
 */
void statw(const char *restrict path, struct stat *restrict buf) {
    if (stat(path, buf) < 0) {
        perror("stat");
        exit(1);
    }
}

/**
 * These  functions return information about a link, in the buffer pointed to by stat.
 * if lstat fail then call perror and exit
 * @param path path file to lstat
 * @param buf buffer to store result
 */
void lstatw(const char *path, struct stat *buf) {
    if (lstat(path, buf) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }
}

/**
 * The  mallocw()  function  allocates size bytes and returns a pointer to the allocated memory.
 * if malloc fail then call display error and exit
 * @param size
 * @return pointer to address
 */
void *mallocw(size_t size) {
    void *pointer = malloc(size);
    if (pointer == NULL) {
        fprintf(stderr, "insufficient memory\n");
        exit(EXIT_FAILURE);
    }
    return pointer;
}

/**
 * The  readlinkw() function shall place the contents of the symbolic link
 * referred to by path in the buffer buf which has size bufsize. If the
 * number of bytes in the symbolic link is less than bufsize, the contents
 * of the remainder of buf are unspecified. If the buf argument is not
 * large enough to contain the link content, the first bufsize bytes shall be placed in buf.
 * if readlink fail then call perror and exit
 * @param path path of the symbolic link
 * @param buf buffer to save the result
 * @param bufsiz the size of the buffer
 */
void readlinkw(const char *path, char *buf, size_t bufsiz) {
    ssize_t r = readlink(path, buf, bufsiz);
    if (r == -1) {
        perror("readlink");
        exit(EXIT_FAILURE);
    }
}

/**
 * symlinkw() creates a symbolic link named linkpath which contains the string target.
 * if symlink fail then call perror and exit
 * @param target the path of the target
 * @param linkpath the path of the symlink
 */
void symlinkw(const char *target, const char *linkpath) {
    int s = symlink(target, linkpath);
    if (s == -1) {
        perror("symlink");
        exit(42);
    }
}

/**
 * The rmdir() function shall remove a directory whose name is given by path.
 * The directory shall be removed only if it is an empty directory.
 * if rmdir fail then call perror and exit
 * @param path the path of the directory to be deleted
 */
void rmdirw(const char *path) {
    int r = rmdir(path);
    if (r == -1) {
        perror("rmdir");
        exit(42);
    }
}

/**
 * The  unlinks()  function shall remove a link to a file.
 * If path names a symbolic link, unlink() shall remove the symbolic link named by path
 * and shall not affect any file or directory named by the contents of the symbolic link.
 * if unlink fail then call perror and exit
 * @param pathname file to unlink
 */
void unlinkw(const char *pathname) {
    int un = unlink(pathname);
    if (un == -1) {
        perror("unlink");
        exit(42);
    }
}

/**
 * ftw()  walks  through  the  directory tree that is located under the directory dirpath,
 * and calls fn() once for each entry in the tree.  By default, directories are handled
 * before the files and subdirectories they contain (preorder traversal).
 * if nftw fail then call perror and exit
 * @param dirpath the path to explore
 * @param fn the function callback
 * @param nopenfd max directory open at the same time
 * @param flags
 */
void nftww(const char *dirpath, int (*fn)(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf),
           int nopenfd, int flags) {
    if (nftw(dirpath, fn, nopenfd, flags) == -1) {
        perror("nftw");
        exit(EXIT_FAILURE);
    }
}

/**
 * The  mkdir()  function shall create a new directory with name path.
 * if mkdir fail then perror and exit
 * @param path the path of the directory to be created
 * @param mode the permissions
 */
void mkdirw(const char *path, mode_t mode) {
    if (mkdir(path, mode) == -1) {
        if (errno != EEXIST) {
            perror("mkdir");
            exit(2);
        }
    }
}

/**
 * The  kill()  function  shall send a signal to a process or
 * a group of processes specified by pid. The signal to be sent is specified by sig
 * and is either one from the list given in <signal.h> or 0.
 * if fail the call perror and exit
 * @param pid to kill
 * @param sig signal to send
 */
void killw(pid_t pid, int sig) {
    if (kill(pid, sig) == -1) {
        perror("kill");
        exit(2);
    }
}
