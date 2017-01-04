//
// Created by matoran on 12/16/16.
//

#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include "filter.h"
#include "logger.h"

void initFilter(){
    filterConditions = NULL;
}

void filter(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf){
    //size, path, filename
    printf("\nfilename \t%s\n",  fpath + ftwbuf->base);
    printf("path \t%s\n", fpath);
    printf("size \t%7jd\n", (intmax_t) sb->st_size);



    /*printf("%-3s %7jd   %-40s %s\n",
           (tflag == FTW_D) ? "d" :
           (tflag == FTW_DNR) ? "dnr" :
           (tflag == FTW_DP) ? "dp" :
           (tflag == FTW_F) ? "f" :
           (tflag == FTW_NS) ? "ns" :
           (tflag == FTW_SL) ? "sl" :
           (tflag == FTW_SLN) ? "sln" :
           "???",
           //ftwbuf->level,
           (intmax_t) sb->st_size,
           fpath,
           //ftwbuf->base,
           fpath + ftwbuf->base);*/
    //printf("File Size: \t\t%d bytes\n",bufstat.st_size);
    //printf("Number of Links: \t%d\n",bufstat.st_nlink);
    //printf("File inode: \t\t%d\n",bufstat.st_ino);


    //Time
    struct stat bufstat;
    if(stat(fpath, &bufstat) < 0){
        logFile("error stat file");
        exit(1);
    }
    char buff[20];
    strftime(buff, 20, "%Y/%m/%d", localtime(&bufstat.st_atime));
    printf("Date \t%s\n", buff);

    //File Permissions
    printf("file Permissions \t");
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

    //group and user
    struct passwd *pw = getpwuid(bufstat.st_uid);
    struct group  *gr = getgrgid(bufstat.st_gid);
    //printf("%d",bufstat.st_uid);
    if(pw != 0)
        printf("user \t%s\n", pw->pw_name);
        /*pw = getpwnam(pw->pw_name);
        if(pw != 0)
            printf("%d ",pw->pw_uid);*/
    if(gr != 0)
        printf("groupe \t%s", gr->gr_name);

    printf("\n\n");



    //printf("The file %s a symbolic link\n", (S_ISLNK(bufstat.st_mode)) ? "is" : "is not");
}
