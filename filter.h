//
// Created by matoran on 12/16/16.
//

#ifndef SEARCHFOLDER_FILTER_H
#define SEARCHFOLDER_FILTER_H
#define PLUS 0
#define MINUS 1
#define EQUAL 2
#define STATUS 0
#define ACCESSED 1
#define MODIFIED 2
#define USER 0
#define GROUP 1

#include <time.h>
#include <stdbool.h>

typedef struct dataFilterS{
    int type;
    int data;
    int symbol;
}dataFilterS;

typedef struct sizeS{
    int symbol;
    long long number;
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

void **filterConditions;
int *exp;
int size;

void initFilter();
void filter(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf);

#endif //SEARCHFOLDER_FILTER_H
