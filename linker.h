/**
 * \author ISELI Cyril & RODRIGUES Marco
 * \date December 2016 and January 2017
 */
#ifndef SEARCHFOLDER_LINKER_H
#define SEARCHFOLDER_LINKER_H

/**
 * If the file is not linked in the smartfolder.
 */
#define SYMLINK_EXIST 0
/**
 * If the symlink is the same as another.
 */
#define SAME_SYMLINK 1
/**
 * If the filename is same as another but not the symlink.
 */
#define SYMLINK_NO_EXIST 2

char* linker_destination;
void zelda(const char *linkSource, const char *filename);
#endif //SEARCHFOLDER_LINKER_H
