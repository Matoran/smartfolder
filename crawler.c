//
// Created by matoran on 12/16/16.
//

#include "crawler.h"

static int display_info(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf) {
    printf("%-3s %2d %7jd   %-40s %d %s\n",
           (tflag == FTW_D) ? "d" : (tflag == FTW_DNR) ? "dnr" :
                                    (tflag == FTW_DP) ? "dp" : (tflag == FTW_F) ? "f" :
                                                               (tflag == FTW_NS) ? "ns" : (tflag == FTW_SL) ? "sl" :
                                                                                          (tflag == FTW_SLN) ? "sln"
                                                                                                             : "???",
           ftwbuf->level, (intmax_t) sb->st_size,
           fpath, ftwbuf->base, fpath + ftwbuf->base);
    return 0;           /* To tell nftw() to continue */
}

void crawler_launcher(const char *path) {
    if (nftw(path, display_info, 20, 0) == -1) {
        perror("nftw");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}