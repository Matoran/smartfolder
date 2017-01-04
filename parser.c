//
// Created by matoran on 12/16/16.
//

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

static int isDirectory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

int isLogicGate(const char *string) {
    if (strcmp(string, "NOT") == 0)
        return NOT;
    else if (strcmp(string, "OR") == 0)
        return OR;
    else if (strcmp(string, "AND") == 0)
        return AND;
    else if (strcmp(string, "(") == 0)
        return BRACKET_OPEN;
    else if (strcmp(string, ")") == 0)
        return BRACKET_CLOSE;

    return -1;
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
    nameS *name = malloc(sizeof(nameS));
    char *token = strtok(NULL, " ");
    name->exactName = token[0] != '"';
    name->string = token;
    return name;
}

void *parseSize() {
    sizeS *size = malloc(sizeof(sizeS));
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
        size->number = size->number*1073741824*1024; //pow(1024, 4)
    } else if (strcmp(token, "GB") == 0) {
        size->number = size->number*1073741824; //pow(1024, 3)
    } else if (strcmp(token, "MB") == 0) {
        size->number = size->number*1048576; //pow(1024, 2)
    } else if (strcmp(token, "KB") == 0) {
        size->number = size->number*1024;
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
    date->date.tm_mon = atoi(strtok(NULL, "."))-1;
    date->date.tm_year = atoi(strtok(NULL, " ."))-1900;

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
    if(owner->type == USER){
        owner->number = getpwnam(token)->pw_uid;
    }else{
        owner->number = getgrnam(token)->gr_gid;
    }
    return owner;
}

void *parsePerm() {
    permS *perm = malloc(sizeof(permS));
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
    }else {
        perm->symbol = PLUS;
    }
    perm->number = strtol(token, NULL, 8);
    return perm;
}

void parser(int argc, char *argv[]) {
    debug("parser begin");
    if (argc < 2) {
        printf("usage: ./smartfolder destination source");
    }
    if(mkdir(argv[1], S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) == -1){
        if(errno != EEXIST){
            logFile("parser error creation directory");
        }
    }
    if (isDirectory(argv[1])) {
        linker_destination = argv[1];
    } else {

        printf("error: destination is not a directory\n");
    }

    if (isDirectory(argv[2])) {
        //linker_destination = argv[2];
    } else {
        printf("error: source is not a directory\n");
    }
    initFilter();


    //Algorithm Shunting-yard
    stackS *stack;
    createStack(&stack);
    int logicGate;
    //simulate bracket open
    int *bracketOpen = malloc(sizeof(int));
    *bracketOpen = BRACKET_OPEN;
    push(&stack, bracketOpen);
    char *expression = malloc(sizeof(char) * (argc - 3) * 255);
    int length = 0;
    int countTotal = 0, countConditions = 0;
    for (int i = 3; i < argc; ++i) {
        if ((logicGate = isLogicGate(argv[i])) >= 0) {
            if (logicGate == BRACKET_CLOSE) {
                while (!isEmpty(stack) && (logicGate = *((int *) pop(&stack))) != BRACKET_OPEN) {
                    length += sprintf(expression + length, "%d ", logicGate);
                    countTotal++;
                }
            } else {
                int *value = malloc(sizeof(int));
                *value = logicGate;
                push(&stack, value);

            }
        } else {
            length += sprintf(expression + length, "%s ", argv[i]);
            if (argv[i][0] == '-') {
                countTotal++;
                countConditions++;
            }

        }
    }
    //simulate bracket close expression is surrounded by bracket open and bracket close
    while (!isEmpty(stack) && (logicGate = *((int *) pop(&stack))) != BRACKET_OPEN) {
        length += sprintf(expression + length, "%d ", logicGate);
        countTotal++;
    }
    printf("expression: %s\n", expression);
    printf("count condition : %d\n", countConditions);
    printf("count total : %d\n", countTotal);
    filterConditions = malloc(sizeof(void*)*countConditions);
    expressionFilter = malloc(sizeof(int)*countTotal);
    //now expression is post fixed
    char *token = strtok(expression, " ");
    int i = 0;
    int j = 0;
    while (token) {
        printf("token: %s\n", token);
        if (token[0] == '-') {
            printf("%d\n", conditionType(token));
            int type = conditionType(token);
            switch (type) {
                case NAMES:
                    filterConditions[i] =  parseName();

                    break;
                case SIZES:
                    filterConditions[i] =  parseSize();
                    break;
                case DATES:
                    filterConditions[i] =  parseDate();
                    break;
                case OWNERS:
                    filterConditions[i] =  parseOwner();
                    break;
                case PERMS:
                    filterConditions[i] =  parsePerm();
                    break;
                default:
                    logFile("error unknown condition");
            }
            expressionFilter[j] = type;
            i++;
        } else {
            expressionFilter[j] = atoi(token);
            printf("OR AND NOT\n");
        }
        token = strtok(NULL, " ");
        j++;
    }

    for (int k = 0; k < countTotal; ++k) {
        printf("%d ", expressionFilter[k]);
    }
    size = countTotal;

    debug("parser end");
}