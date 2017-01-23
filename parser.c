/**
 * \author ISELI Cyril & RODRIGUES Marco
 * \brief ADD Text
 * \version 0.1
 * \date December 2016 and January 2017
 *
 * ADD text
 */

#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include "parser.h"
#include "linker.h"
#include "filter.h"
#include "logger.h"
#include "stack.h"
#include "wrappersyscall.h"

char *getLogicGateName(int val) {
    switch (val) {
        case NOT:
            return "NOT";
        case OR:
            return "OR";
        case AND:
            return "AND";
        case BRACKET_OPEN:
            return "BRACKET OPEN";
        case BRACKET_CLOSE:
            return "BRACKET CLOSE";
        default:
            return "unknow logic gate";
    }
}

static int isDirectory(const char *path) {
    struct stat statbuf;
    statw(path, &statbuf);
    return S_ISDIR(statbuf.st_mode);
}

int isLogicGate(const char *string) {
    if (strcmp(string, "NOT") == 0)
        return NOT;
    else if (strcmp(string, "OR") == 0)
        return OR;
    else if (strcmp(string, "AND") == 0)
        return AND;
    else if (strcmp(string, "[") == 0)
        return BRACKET_OPEN;
    else if (strcmp(string, "]") == 0)
        return BRACKET_CLOSE;

    return -1;
}

bool isWriteable(char *fpath) {
    struct stat bufstat;
    statw(fpath, &bufstat);
    if (bufstat.st_mode & S_IWUSR) {
        return true;
    }
    return false;
}

bool isReadable(char *fpath) {
    struct stat bufstat;
    statw(fpath, &bufstat);
    if (bufstat.st_mode & S_IRUSR) {
        return true;
    }
    return false;
}

int conditionType(const char *string) {
    if (strcmp(string, "-name") == 0)
        return NAMES;
    else if (strcmp(string, "-size") == 0)
        return SIZES;
    else if (strcmp(string, "-date") == 0)
        return DATES;
    else if (strcmp(string, "-owner") == 0)
        return OWNERS;
    else if (strcmp(string, "-perm") == 0)
        return PERMS;
    return -1;
}

void *parseName() {
    nameS *name = mallocw(sizeof(nameS));
    char *token = strtok(NULL, " ");
    if (strcmp(token, "-") == 0) {
        name->exactName = false;
        token = strtok(NULL, " ");
    } else if (strcmp(token, "=") == 0) {
        name->exactName = true;
        token = strtok(NULL, " ");
    } else {
        name->exactName = true;
    }
    name->string = token;
    return name;
}

void *parseSize() {
    sizeS *size = mallocw(sizeof(sizeS));
    //symbol (+, -, =)
    char *token = strtok(NULL, " ");
    if (strcmp(token, "+") == 0) {
        size->symbol = PLUS;
        token = strtok(NULL, " ");
    } else if (strcmp(token, "-") == 0) {
        size->symbol = MINUS;
        token = strtok(NULL, " ");
    } else if (strcmp(token, "=") == 0) {
        size->symbol = EQUAL;
        token = strtok(NULL, " ");
    } else {
        size->symbol = EQUAL;
    }
    //size
    size->number = atoi(token);
    //unit
    token = strtok(NULL, " ");
    if (strcmp(token, "TB") == 0) {
        size->number = size->number * 1073741824 * 1024; //pow(1024, 4)
    } else if (strcmp(token, "GB") == 0) {
        size->number = size->number * 1073741824; //pow(1024, 3)
    } else if (strcmp(token, "MB") == 0) {
        size->number = size->number * 1048576; //pow(1024, 2)
    } else if (strcmp(token, "KB") == 0) {
        size->number = size->number * 1024;
    } else if (token[0] == '-') {

    } else {
        printf("unknow unit:  %s", token);
        exit(2);
    }
    return size;
}

void *parseDate() {
    dateS *date = malloc(sizeof(dateS));
    //type
    char *token = strtok(NULL, " .");
    if (strcmp(token, "status") == 0) {
        date->symbol = STATUS;
        token = strtok(NULL, " .");
    } else if (strcmp(token, "modified") == 0) {
        date->symbol = MODIFIED;
        token = strtok(NULL, " .");
    } else if (strcmp(token, "accessed") == 0) {
        date->symbol = ACCESSED;
        token = strtok(NULL, " .");
    } else {
        date->symbol = ACCESSED;
    }

    //symbol
    if (strcmp(token, "+") == 0) {
        date->symbol = PLUS;
        token = strtok(NULL, " .");
    } else if (strcmp(token, "-") == 0) {
        date->symbol = MINUS;
        token = strtok(NULL, " .");
    } else if (strcmp(token, "=") == 0) {
        date->symbol = EQUAL;
        token = strtok(NULL, " .");
    } else {
        date->symbol = EQUAL;
    }

    //date
    date->date.tm_mday = atoi(token);
    date->date.tm_mon = atoi(strtok(NULL, ".")) - 1;
    date->date.tm_year = atoi(strtok(NULL, " .")) - 1900;

    return date;
}

void *parseOwner() {
    ownerS *owner = malloc(sizeof(ownerS));

    char *token = strtok(NULL, " ");
    if (strcmp(token, "group") == 0) {
        owner->type = GROUP;
        token = strtok(NULL, " ");
    } else if (strcmp(token, "user") == 0) {
        owner->type = USER;
        token = strtok(NULL, " ");
    } else {
        owner->type = USER;
    }
    if (owner->type == USER) {
        owner->number = getpwnam(token)->pw_uid;
    } else {
        owner->number = getgrnam(token)->gr_gid;
    }
    return owner;
}

void *parsePerm() {
    permS *perm = mallocw(sizeof(permS));
    char *token = strtok(NULL, " ");
    if (strcmp(token, "+") == 0) {
        perm->symbol = PLUS;
        token = strtok(NULL, " ");
    } else if (strcmp(token, "-") == 0) {
        perm->symbol = MINUS;
        token = strtok(NULL, " ");
    } else if (strcmp(token, "=") == 0) {
        perm->symbol = EQUAL;
        token = strtok(NULL, " ");
    } else {
        perm->symbol = PLUS;
    }
    perm->number = strtol(token, NULL, 8);
    return perm;
}

void parser(int argc, char *argv[]) {
    logger("parser begin\n", DEBUG, true);
    if (mkdir(argv[1], S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) == -1) {
        if (errno != EEXIST) {
            perror("mkdir destination");
            exit(3);
        }
    }
    logger("destination %s\n", DEBUG, true, argv[1]);
    linker_destination = realpath(argv[1], NULL);
    logger("realpath destination %s\n", DEBUG, true, linker_destination);
    if (isDirectory(linker_destination)) {
        if (!isWriteable(linker_destination)) {
            logger("no write access to destination\n", ERROR, true);
            exit(4);
        }
    } else {
        logger("destination is not a directory\n", ERROR, true);
        exit(5);
    }

    if (isDirectory(argv[2])) {
        if (!isReadable(argv[2])) {
            logger("source cannot be read\n", ERROR, true);
            exit(6);
        }
    } else {
        logger("source is not a directory\n", ERROR, true);
        exit(7);
    }
    initFilter();

    logger("parse boolean expression with shunting-yard\n", DEBUG, true);
    //Algorithm Shunting-yard
    stackS *stack;
    createStack(&stack);
    int logicGate;
    //simulate bracket open
    int *bracketOpen = mallocw(sizeof(int));
    *bracketOpen = BRACKET_OPEN;
    push(&stack, bracketOpen);
    char *expression = mallocw(sizeof(char) * (argc - 3) * 255);
    expression[0] = '\0';
    int length = 0;
    int countTotal = 0, countConditions = 0;
    bool popAfterCondition = false, condition = false;
    for (int i = 3; i < argc; ++i) {
        if ((logicGate = isLogicGate(argv[i])) >= 0) {
            logger("%s is a logic gate\n", DEBUG, true, argv[i]);
            if (logicGate == BRACKET_CLOSE) {
                logger("%s is a bracket close\n", DEBUG, true, argv[i]);
                while (!isEmpty(stack) && (logicGate = *((int *) pop(&stack))) != BRACKET_OPEN) {
                    logger("pop logic gate %s\n", DEBUG, true, getLogicGateName(logicGate));
                    length += sprintf(expression + length, "%d ", logicGate);
                    countTotal++;
                }
                logger("actual expression %s\n", DEBUG, true, expression);
            } else {
                if (logicGate == NOT && isLogicGate(argv[i + 1]) != BRACKET_OPEN) {
                    popAfterCondition = true;
                }
                logger("%s push in stack\n", DEBUG, true, argv[i]);
                int *value = mallocw(sizeof(int));
                *value = logicGate;
                push(&stack, value);
            }
        } else {
            logger("%s is not a logic gate\n", DEBUG, true, argv[i]);
            length += sprintf(expression + length, "%s ", argv[i]);

            logger("actual expression %s\n", DEBUG, true, expression);
            if (argv[i][0] == '-') {
                countTotal++;
                countConditions++;
                if (popAfterCondition && condition) {
                    length += sprintf(expression + length, "%d ", *((int *) pop(&stack)));
                    popAfterCondition = false;
                    condition = false;
                }
                condition = true;
            }

        }
    }
    //simulate bracket close expression is surrounded by bracket open and bracket close
    while (!isEmpty(stack) && (logicGate = *((int *) pop(&stack))) != BRACKET_OPEN) {
        length += sprintf(expression + length, "%d ", logicGate);
        countTotal++;
    }
    logger("actual expression \"%s\", count conditions %d, count total %d\n", DEBUG, true, expression, countConditions,
           countTotal);
    exit(42);
    filterConditions = mallocw(sizeof(void *) * countConditions);
    expressionFilter = mallocw(sizeof(int) * countTotal);
    //now expression is post fixed
    char *token = strtok(expression, " ");
    int i = 0;
    int j = 0;
    while (token) {
        logger("token %s\n", DEBUG, true, token);
        if (token[0] == '-') {
            printf("%d\n", conditionType(token));
            int type = conditionType(token);
            switch (type) {
                case NAMES:
                    filterConditions[i] = parseName();
                    break;
                case SIZES:
                    filterConditions[i] = parseSize();
                    break;
                case DATES:
                    filterConditions[i] = parseDate();
                    break;
                case OWNERS:
                    filterConditions[i] = parseOwner();
                    break;
                case PERMS:
                    filterConditions[i] = parsePerm();
                    break;
                default:
                    logger("error unknown condition\n", ERROR, true);
            }
            expressionFilter[j] = type;
            i++;
        } else {
            expressionFilter[j] = atoi(token);
            logger("token is a logic gate %s\n", DEBUG, true, getLogicGateName(expressionFilter[j]));
        }
        token = strtok(NULL, " ");
        j++;
    }

    for (int k = 0; k < countTotal; ++k) {
        printf("%d ", expressionFilter[k]);
    }
    size = countTotal;
    logger("parser end\n", DEBUG, true);
}