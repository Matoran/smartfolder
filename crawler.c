//
// Created by matoran on 12/16/16.
//

#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include <stdint.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include "crawler.h"
#include "logger.h"


/*
FTW_F  fpath is a regular file.

FTW_D  fpath is a directory.

FTW_DNR fpath is a directory which can't be read.

FTW_NS The stat(2) call failed on fpath, which is not a symbolic link.

FTW_DP fpath is a directory, and FTW_DEPTH was specified in flags.  All of the files and subdirectories  within  fpath
                have  been  processed.

FTW_SL fpath is a symbolic link, and FTW_PHYS was set in flags.

FTW_SLN fpath  is a symbolic link pointing to a nonexistent file.  (This occurs only if FTW_PHYS is not set.)
*/


static int display_info(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf) {
    printf("%-3s %2d %7jd   %-40s %d %s\n",
           (tflag == FTW_D) ? "d" :
           (tflag == FTW_DNR) ? "dnr" :
           (tflag == FTW_DP) ? "dp" :
           (tflag == FTW_F) ? "f" :
           (tflag == FTW_NS) ? "ns" :
           (tflag == FTW_SL) ? "sl" :
           (tflag == FTW_SLN) ? "sln" :
           "???",
           ftwbuf->level,
           (intmax_t) sb->st_size,
           fpath,
           ftwbuf->base,
           fpath + ftwbuf->base);
    struct stat bufstat;
    if(stat(fpath, &bufstat) < 0){
        log("error stat file");
        return 1;
    }


    //printf("File Size: \t\t%d bytes\n",bufstat.st_size);
    //printf("Number of Links: \t%d\n",bufstat.st_nlink);
    //printf("File inode: \t\t%d\n",bufstat.st_ino);
    char buff[20];
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&bufstat.st_atime));
    printf(buff);
    printf("\nFile Permissions: \t");
    //printf( (S_ISDIR(bufstat.st_mode)) ? "d" : "-");
    printf( (bufstat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (bufstat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (bufstat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (bufstat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (bufstat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (bufstat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (bufstat.st_mode & S_IROTH) ? "r" : "-");
    printf( (bufstat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (bufstat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");
    struct passwd *pw = getpwuid(bufstat.st_uid);
    struct group  *gr = getgrgid(bufstat.st_gid);
    //printf("%d",bufstat.st_uid);
    if(pw != 0)
        printf("user %s", pw->pw_name);
    if(gr != 0)
        printf("groupe %s", gr->gr_name);

    printf("\n\n");



    //printf("The file %s a symbolic link\n", (S_ISLNK(bufstat.st_mode)) ? "is" : "is not");


    return 0;           /* To tell nftw() to continue */
}

void crawler_launcher(const char *path) {
    if (nftw(path, display_info, 20, 0) == -1) {
        perror("nftw");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}