/** @brief Filter a file
 * @author ISELI Cyril & RODRIGUES Marco
 * @version 0.1
 * @date December 2016 and January 2017
 * @file filter.c
 *
 * Filter a file with the parameters in command line.
 */


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
#include "wrappersyscall.h"

/**
 * Check if the name is same the parameters.
 * The name can be partial or not.
 * @param pos position in the postfixed expression
 * @param fpath path of the file
 * @return bool if name is valid
 */
bool checkName(int pos, const char *fpath){
    nameS *condition = ((nameS*)filterConditions[pos]);
    if(condition->exactName){
        return strcmp(condition->string, fpath) == 0;
    }else{
        return strstr(fpath, condition->string) != NULL;
    }
}

/**
 * Check if the size is same, smaller or bigger than the parameters.
 * @param pos position in the postfixed expression
 * @param size size of file
 * @return bool if size is valid
 */
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
            logger("checkSize error condition symbol unknow %d\n", ERROR, true, condition->symbol);
            exit(1);
    }
}

/**
 * Check if the date is same, smaller or bigger than the parameters.
 * The dates are in 3 modes, Status, Accessed or Modified.
 * @param pos position in the postfixed expression
 * @param sb is the stat of file
 * @return bool if date is valid
 */
bool checkDate(int pos, const struct stat *sb){
    dateS *condition = ((dateS*)filterConditions[pos]);
    time_t time;
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
            logger("checkDate error condition type unknow %d\n", ERROR, true, condition->type);
            exit(1);
    }

    switch (condition->symbol){
        case PLUS:
            logger("time %d > condition %d\n", DEBUG, true, time, condition->timestamp + ONE_DAY);
            return time >= condition->timestamp + ONE_DAY;
        case MINUS:
            logger("time %d < condition %d\n", DEBUG, true, time, condition->timestamp);
            return time < condition->timestamp;
        case EQUAL:
            logger("time %d >= condition %d and < %d\n", DEBUG, true, time, condition->timestamp,
                   condition->timestamp + ONE_DAY);
            return time >= condition->timestamp && time < condition->timestamp + ONE_DAY;
        default:
            logger("checkDate error condition symbol unknow %d\n", ERROR, true, condition->symbol);
            exit(1);
    }
}

/**
 * Check if the file belongs to a group or user of parameters.
 * @param pos position in the postfixed expression
 * @param sb is the stat of file
 * @return bool if group or user is valid
 */
bool checkOwner(int pos, const struct stat *sb){
    ownerS *condition = ((ownerS*)filterConditions[pos]);
    switch (condition->type){
        case GROUP:
            return condition->number == sb->st_gid;
        case USER:
            return condition->number == sb->st_uid;
        default:
            logger("checkOwner error condition type unknow %d\n", ERROR, true, condition->type);
            exit(1);
    }
}

/**
 * Check if the permission is the same, smaller or bigger than parameters.
 * @param pos position in the postfixed expression
 * @param sb is the stat of file
 * @return bool if permission is valid
 */
bool checkPerm(int pos, const struct stat *sb){
    permS *condition = ((permS*)filterConditions[pos]);
    int perms = sb->st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
    switch (condition->symbol){
        case PLUS:
            return perms >= condition->number;
        case MINUS:
            return (perms & condition->number) > 0;
        case EQUAL:
            return perms == condition->number;
        default:
            logger("checkPerm error condition symbol unknow %d\n", ERROR, true, condition->symbol);
            exit(1);
    }
}

/**
 * Initialize the global variable: filterConditions.
 */
void initFilter(){
    filterConditions = NULL;
}

/**
 * Filter a file based on the parameters in the command line.
 * Call the function zelda(linker) with the filtered file.
 * @param fpath path of the file
 * @param sb stat of the file
 * @param tflag flag of the file
 * @param ftwbuf
 */
void filter(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf){
    logger("filter begin\n", DEBUG, true);
    stackBoolS *stack = NULL;
    int j = 0;
    logger("expression: ", DEBUG, true);
    for (int i = 0; i < size; ++i) {
        logger("%d ", DEBUG, false, expressionFilter[i]);
        logger(" j=%d", DEBUG, false, j);
        switch (expressionFilter[i]){
            case NOT:
                pushBool(&stack, !popBool(&stack));
                logger("(NOT) ", DEBUG, false);
                break;
            case OR:
                pushBool(&stack, popBool(&stack) | popBool(&stack));
                logger("(OR) ", DEBUG, false);
                break;
            case AND:
                pushBool(&stack, popBool(&stack) & popBool(&stack));
                logger("(AND) ", DEBUG, false);
                break;
            case NAMES:
                pushBool(&stack, checkName(j, fpath + ftwbuf->base));
                logger("(NAME) ", DEBUG, false);
                j++;
                break;
            case SIZES:
                pushBool(&stack, checkSize(j, sb->st_size));
                logger("(SIZE) ", DEBUG, false);
                j++;
                break;
            case DATES:
                pushBool(&stack, checkDate(j, sb));
                logger("(DATE) ", DEBUG, false);
                j++;
                break;
            case OWNERS:
                pushBool(&stack, checkOwner(j, sb));
                logger("(OWNER) ", DEBUG, false);
                j++;
                break;
            case PERMS:
                pushBool(&stack, checkPerm(j, sb));
                logger("(PERM) ", DEBUG, false);
                j++;
                break;
            default:
                logger("error in filter, unknow expression type %d\n", ERROR, true, expressionFilter[i]);
        }
        displayStackBool(stack);
    }
    if(size == 0 || popBool(&stack)){
        logger("file valid add link\n", DEBUG, true);
        zelda(fpath, fpath + ftwbuf->base);
    }

    logger("filter end\n", DEBUG, true);
}
