//
// Created by matoran on 12/16/16.
//

#ifndef SEARCHFOLDER_FILTER_H
#define SEARCHFOLDER_FILTER_H
#define PLUS 0
#define MINUS 1
#define EQUAL 2

#include <time.h>
#include "circ_list.h"

typedef struct dataFilterS{
    int type;
    int data;
    int symbol;
}dataFilterS;

typedef struct sizeS{
    int symbol;
    int number;
    int unit;
}sizeS;

typedef struct dateS{
    int type;
    int symbol;
    struct tm date;
}dateS;

typedef struct ownerS{
    int type;
    int number;
}ownerS;

typedef struct permS{
    int symbol;
    int number;
}permS;

typedef struct nameS{
    bool exactName;
    char *string;
}nameS;

circListS *filterConditions;

void initFilter();
void filter(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf);

#endif //SEARCHFOLDER_FILTER_H
