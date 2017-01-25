/**
 * @author ISELI Cyril & RODRIGUES Marco
 * @brief Delete smartfolder and check validity links
 * @version 0.1
 * @date December 2016 and January 2017
 * @file destroyer.h
 *
 * Delete smartfolder when the user launch the -d <smartfolder name>
 * Check validity of each link, if the link point to a deleted file, then delete the link
 */

#ifndef SEARCHFOLDER_DESTROYER_H
#define SEARCHFOLDER_DESTROYER_H

void destroy(char *path);
void checkFiles(const char *path);

#endif //SEARCHFOLDER_DESTROYER_H
