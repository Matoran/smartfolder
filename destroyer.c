/**
 * @author ISELI Cyril & RODRIGUES Marco
 * @brief Delete smartfolder and check validity links
 * @version 0.1
 * @date December 2016 and January 2017
 * @file destroyer.c
 *
 * Delete smartfolder when the user launch the -d <smartfolder name>
 * Check validity of each link, if the link point to a deleted file, then delete the link
 */

/**
 * active gnu source to use versionsort
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <ftw.h>
#include <string.h>
#include <ctype.h>
#include "destroyer.h"
#include "logger.h"
#include "wrappersyscall.h"

/**
 * Check if is a symlink
 * @param name file
 * @return int 1 if is valid
 */
int scanFilter(const struct dirent *name) {
    if (name->d_type == DT_LNK) {
        return 1;
    }
    return 0;
}

/**
 * Get the number at the end of file.
 * @param string filename of symlink
 * @return int the number of file
 */
int getInt(const char *string) {
    size_t length = strlen(string);
    int i = 0;
    while (isdigit(string[length - i - 1])) {
        i++;
        if (i > 8)
            break;
    }
    if (i > 0) {
        return atoi(&string[length - i]);
    } else {
        return 0;
    }
}

/**
 * Remove symlink or directory.
 * @param fpath path of the file
 * @param sb stat of the file
 * @param tflag flag of the file
 * @param ftwbuf
 * @return int 0 for continue nftw
 */
static int removeLink(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf) {
    if (tflag != FTW_DP) {
        unlinkw(fpath);
    } else {
        rmdirw(fpath);
    }
    return 0;
}

/**
 * Walks through the directory and remove all the symlink.
 * of searchfolder.
 * @param path path of searchfolder
 */
void destroy(char *path) {
    nftww(path, removeLink, 64, FTW_DEPTH | FTW_PHYS);
}

/**
 * Check if the file of symlink is alive
 * @param fpath path of the synlink
 * @return int 1 if is valid
 */
static int stillAlive(const char *fpath) {
    logger("path %s\n", DEBUG, true, fpath);
    struct stat sb;
    char *linkname;
    lstatw(fpath, &sb);
    linkname = mallocw(sb.st_size + 1);
    readlink(fpath, linkname, sb.st_size + 1);
    linkname[sb.st_size] = '\0';
    logger("linkname %s\n", DEBUG, true, linkname);
    return stat(linkname, &sb);
}

/**
 * Check all symlinks in path and if the symlink is not live
 * delete him.
 * @param path path of the smartfolder
 */
void checkFiles(const char *path) {
    logger("check files begin %s\n", DEBUG, true, path);
    struct dirent **namelist;
    chdir(path);
    int n = scandir(".", &namelist, scanFilter, versionsort);
    int first = n - 1;
    int distance = 0;
    if (n < 0)
        perror("scandir");
    else {
        while (n--) {
            if (n > 0) {
                int a = getInt(namelist[n]->d_name);
                int b = getInt(namelist[n - 1]->d_name);
                distance = abs(a - b);
                if ((a == 1 && b == 0) || (b == 1 && a == 0)) {
                    distance = 1;
                }
                if (distance > 1) {
                    first = n - 1;
                }
            }
            if (stillAlive(namelist[n]->d_name)) {
                unlinkw(namelist[n]->d_name);
                if (distance == 1) {
                    rename(namelist[first]->d_name, namelist[n]->d_name);
                    first--;
                }
            }
            //free(namelist[n]);
        }
        free(namelist);
    }
    logger("check files end\n", DEBUG, true);
}