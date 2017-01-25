/** @brief Filter a file
 * @author ISELI Cyril & RODRIGUES Marco
 * @version 0.1
 * @date December 2016 and January 2017
 * @file filter.h
 *
 * Filter a file with the parameters in command line.
 */

#ifndef SEARCHFOLDER_FILTER_H
#define SEARCHFOLDER_FILTER_H
/**
 * It's >
 */
#define PLUS 0
/**
 * It's <
 */
#define MINUS 1
/**
 * It's =
 */
#define EQUAL 2
/**
 * Date mode status
 */
#define STATUS 0
/**
 * Date mode accessed
 */
#define ACCESSED 1
/**
 * Date mode modifier
 */
#define MODIFIED 2
/**
 * If user
 */
#define USER 0
/**
 * If group
 */
#define GROUP 1
#define ONE_DAY (60*60*24)

#include <ftw.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ftw.h>

/**
 * contain a condition about size
 * symbol is +(bigger than) -(smaller than) =(equal)
 * number is the size
 */
typedef struct sizeS{
    int symbol;
    long long number;
}sizeS;

/**
 * contain a condition about date
 * type access time, modification time, status time
 * symbol is +(bigger than) -(smaller than) =(equal)
 * date tm struct that contain the date
 */
typedef struct dateS{
    int type;
    int symbol;
    time_t timestamp;
}dateS;

/**
 * contain a condition about owner
 * type group or user
 * number gid or uid
 */
typedef struct ownerS{
    int type;
    int number;
}ownerS;

/**
 * contain a condition about perms
 * symbol is +(bigger than) -(smaller than) =(equal)
 * number the octal code
 */
typedef struct permS{
    int symbol;
    int number;
}permS;

/**
 * contain a condition about name
 * exactName if we search complete string or partial
 */
typedef struct nameS{
    bool exactName;
    char *string;
}nameS;

/**
 * contain all conditions generic table
 */
void **filterConditions;

/**
 * contain the expression like "NOT dateS AND ownerS"
 */
int *expressionFilter;

/**
 * size of expression filter
 */
int size;

void initFilter();
void filter(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf);

#endif //SEARCHFOLDER_FILTER_H
