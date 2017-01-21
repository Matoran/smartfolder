//
// Created by matoran on 12/16/16.
//

#ifndef SEARCHFOLDER_LINKER_H
#define SEARCHFOLDER_LINKER_H

#define SYMLINK_EXIST 0
#define SAME_SYMLINK 1
#define SYMLINK_NO_EXIST 2

char* linker_destination;
void zelda(const char *linkSource, const char *filename);
#endif //SEARCHFOLDER_LINKER_H
