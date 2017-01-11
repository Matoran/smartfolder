//
// Created by matoran on 12/16/16.
//

#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <memory.h>
#include "filter.h"
#include "logger.h"
#include "parser.h"
#include "stack.h"
#include "linker.h"

bool checkName(int pos, const char *fpath){
    nameS *condition = ((nameS*)filterConditions[pos]);
    if(condition->exactName){
        return strcmp(condition->string, fpath) == 0;
    }else{
        return strstr(fpath, condition->string) != NULL;
    }
}

bool checkSize(int pos, off_t size){
    sizeS *condition = ((sizeS*)filterConditions[pos]);
    switch (condition->symbol){
        case PLUS:
            return size > condition->number;
        case MINUS:
            return size < condition->number;
        case EQUAL:
            return size == condition->number;
        default:
            logFile("checkSize error condition symbol unknow");
            exit(1);
    }
}


bool checkDate(int pos, const struct stat *sb){
    dateS *condition = ((dateS*)filterConditions[pos]);
    time_t time;
    time_t timeCondition = mktime(&condition->date);

    switch (condition->type){
        case STATUS:
            time = sb->st_ctime;
            break;
        case ACCESSED:
            time = sb->st_atime;
            break;
        case MODIFIED:
            time = sb->st_mtime;
            break;
        default:
            logFile("checkDate error condition type unknow");
            exit(1);
    }

    switch (condition->symbol){
        case PLUS:
            return time > timeCondition;
        case MINUS:
            return time < timeCondition;
        case EQUAL:
            return time == timeCondition;
        default:
            logFile("checkDate error condition symbol unknow");
            exit(1);
    }
}

bool checkOwner(int pos, const struct stat *sb){
    ownerS *condition = ((ownerS*)filterConditions[pos]);
    switch (condition->type){
        case GROUP:
            return condition->number == sb->st_gid;
        case USER:
            return condition->number == sb->st_uid;
        default:
            logFile("checkOwner error condition type unknow");
            exit(1);
    }
}

bool checkPerm(int pos, const struct stat *sb){
    permS *condition = ((permS*)filterConditions[pos]);
    int perms = sb->st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
    switch (condition->symbol){
        case PLUS:
            return perms > condition->number;
        case MINUS:
            return perms < condition->number;
        case EQUAL:
            return perms == condition->number;
        default:
            logFile("checkPerm error condition symbol unknow");
            exit(1);
    }
}

void initFilter(){
    filterConditions = NULL;
}

void filter(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf){
    stackBoolS *stack;
    createStackBool(&stack);

    int j = 0;
    printf("\n");
    for (int k = 0; k < size; ++k) {
        printf("%d ", expressionFilter[k]);
    }
    for (int i = 0; i < size; ++i) {
        switch (expressionFilter[i]){
            case NOT:
                pushBool(&stack, !popBool(&stack));
                break;
            case OR:
                pushBool(&stack, popBool(&stack) | popBool(&stack));
                break;
            case AND:
                pushBool(&stack, popBool(&stack) & popBool(&stack));
                break;
            case NAMES:
                pushBool(&stack, checkName(j, fpath + ftwbuf->base));
                j++;
                break;
            case SIZES:
                pushBool(&stack, checkSize(j, sb->st_size));
                j++;
                break;
            case DATES:
                pushBool(&stack, checkDate(j, sb));
                j++;
                break;
            case OWNERS:
                pushBool(&stack, checkOwner(j, sb));
                j++;
                break;
            case PERMS:
                pushBool(&stack, checkPerm(j, sb));
                j++;
                break;
            default:
                logFile("error in filter, unknow expression type");
        }
        displayStackBool(stack);
        printf("\n");
    }
    //printf("valid: %d", popBool(&stack));
    //size, path, filename
    if(size == 0 || popBool(&stack)){
        zelda(fpath, fpath + ftwbuf->base);
    }
    printf("\nfilename \t%s\n",  fpath + ftwbuf->base);
    printf("path \t%s\n", fpath);
    printf("size \t%7jd\n", (intmax_t) sb->st_size);

    //Time
    struct stat bufstat;
    if(stat(fpath, &bufstat) < 0){
        logFile("error stat file");
        exit(1);
    }
    char buff[20];
    strftime(buff, 20, "%Y/%m/%d", localtime(&bufstat.st_atime));
    printf("Date \t%s\n", buff);

    //File Permissions
    printf("file Permissions \t");
    printf("%o\n", sb->st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
    //printf( (S_ISDIR(bufstat.st_mode)) ? "d" : "-");
    printf( (bufstat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (bufstat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (bufstat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (bufstat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (bufstat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (bufstat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (bufstat.st_mode & S_IROTH) ? "r" : "-");
    printf( (bufstat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (bufstat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");

    //group and user
    struct passwd *pw = getpwuid(bufstat.st_uid);
    struct group  *gr = getgrgid(bufstat.st_gid);
    //printf("%d",bufstat.st_uid);
    if(pw != 0)
        printf("user \t%s\n", pw->pw_name);
        /*pw = getpwnam(pw->pw_name);
        if(pw != 0)
            printf("%d ",pw->pw_uid);*/
    if(gr != 0)
        printf("groupe \t%s", gr->gr_name);

    printf("\n\n");


}
