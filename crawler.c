/**
 * \author ISELI Cyril & RODRIGUES Marco
 * \brief ADD Text
 * \version 0.1
 * \date December 2016 and January 2017
 *
 * ADD text
 */
#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdlib.h>
#include <zconf.h>
#include "crawler.h"
#include "logger.h"
#include "filter.h"
#include "wrappersyscall.h"


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
        logger("crawler found file %s\n", DEBUG, true, fpath);
        filter(fpath, sb, tflag, ftwbuf);
    }
    return 0;           /* To tell nftw() to continue */
}

void crawler_launcher(const char *path) {
    logger("crawler begin %s\n", DEBUG, true, path);
    nftww(path, display_info, 20, 0);
    logger("crawler end\n", DEBUG, true);
}