/**
 * @author ISELI Cyril & RODRIGUES Marco
 * @brief Create the symlink
 * @version 0.1
 * @date December 2016 and January 2017
 * @file linker.c
 *
 * Will create the symlink in the smartfolder with
 * the file received from the filter. If any
 * file have the same name, we add a number in the
 * end of file.
 *
 */

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "linker.h"
#include "logger.h"
#include "wrappersyscall.h"

/**
 * Test if the file is existing or not
 * and if the symlink is the same as another
 * @param linkSource path of filtered file
 * @param filename name of filtered file
 * @return int: SYMLINK_NO_EXIST if the file is not linked in the smartfolder
 *              SAME_SYMLINK if the symlink is the same as another
 *              SYMLINK_EXIST if the filename is same as another but not the symlink
 */
int exist(const char *linkSource, char *linkDestination){
    struct stat sb;
    char *linkname;

    if(lstat(linkDestination, &sb) == -1){
        return SYMLINK_NO_EXIST;
    }
    linkname = mallocw(sb.st_size + 1);
    readlinkw(linkDestination, linkname, sb.st_size + 1);
    linkname[sb.st_size] = '\0';

    if(strcmp(linkSource,linkname) == 0){
        free(linkname);
        return SAME_SYMLINK;
    }
    free(linkname);
    return SYMLINK_EXIST;
}

/**
 * Create the symlink between the file filtered and
 * the file in the smart folder
 * @param linkSource path of filtered file
 * @param filename name of filtered file
 */
void zelda(const char *linkSource, const char *filename){
    logger("linker begin\n", DEBUG, true);
    char *linkDestination = mallocw(sizeof(char)*(strlen(linker_destination)+strlen(filename)+10));
    strcpy(linkDestination, linker_destination);
    strcat(linkDestination, "/");
    strcat(linkDestination, filename);
    strcat(linkDestination, ".sl");
    logger("default link destination %s\n", DEBUG, true, linkDestination);
    int i = 0;
    bool open = true;
    size_t length = strlen(linkDestination);
    int valueExist;
    do{
        valueExist = exist(linkSource,linkDestination);
        if(valueExist == SYMLINK_EXIST) {
            i++;
            sprintf(&linkDestination[length], "%d", i);
            logger("name occuped try %s\n", DEBUG, true, linkDestination);
        }else if(valueExist == SYMLINK_NO_EXIST){
            open = false;
            logger("name %s is ok\n", DEBUG, true, linkDestination);
        }else{
            logger("file is already linked\n", DEBUG, true);
            logger("linker end\n", DEBUG, true);
            return;
        }
    }while(open);

    logger("link source %s link destination %s\n", DEBUG, true, linkSource, linkDestination);

    symlinkw(linkSource, linkDestination);
    free(linkDestination);
    logger("linker end\n", DEBUG, true);
}