//
// Created by matoran on 12/16/16.
//

#define _XOPEN_SOURCE 500
#include <stdlib.h>
#include <ftw.h>
#include "crawler.h"
#include "logger.h"
#include "filter.h"


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
    //is a file
    if(tflag == FTW_F){
        filter(fpath, sb, tflag, ftwbuf);
    }
    return 0;           /* To tell nftw() to continue */
}

void crawler_launcher(const char *path) {
    if (nftw(path, display_info, 20, 0) == -1) {
        logFile("nftw error");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}