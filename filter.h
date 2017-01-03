//
// Created by matoran on 12/16/16.
//

#ifndef SEARCHFOLDER_FILTER_H
#define SEARCHFOLDER_FILTER_H

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
    int type; //0 => nom exact, 1 => nom partiel
    char *string;
};

typedef struct filterS{
    int nameNb;
    int sizeNb;
    int dateNb;
    int ownerNb;
    int permNb;
    circListS *names;
    circListS *sizes;
    circListS *dates;
    circListS *owners;
    circListS *perms;
}filterS;

filterS filterConditions;

void initFilter();
#endif //SEARCHFOLDER_FILTER_H