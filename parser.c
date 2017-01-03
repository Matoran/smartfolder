//
// Created by matoran on 12/16/16.
//

#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

int conditionType(const char *string){
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

void parseName(){

}

void parseSize(){

}

void parseDate(){

}

void parseOwner(){

}

void parsePerm(){

}

void parser(int argc, char *argv[]) {
    debug("parser begin");
    if (argc < 2) {
        printf("usage: ./smartfolder destination source");
    }
    if (isDirectory(argv[1])) {
        linker_destination = argv[1];
    } else {
        printf("error: destination is not a directory\n");
    }

    if (isDirectory(argv[2])) {
        linker_destination = argv[2];
    } else {
        printf("error: source is not a directory\n");
    }
    initFilter();
    //Algorithm Shunting-yard
    stackS *stack;
    createStack(&stack);
    int logicGate;
    int *bracketOpen = malloc(sizeof(int));
    *bracketOpen = BRACKET_OPEN;
    push(&stack, bracketOpen);
    char *expression = malloc(sizeof(char)*(argc-3)*30);
    int length = 0;
    int count = 0;
    for (int i = 3; i < argc; ++i) {
        if ((logicGate = isLogicGate(argv[i])) >= 0) {
            if (logicGate == BRACKET_CLOSE) {
                while (!isEmpty(stack) && (logicGate = *((int*)pop(&stack))) != BRACKET_OPEN) {
                    length += sprintf(expression+length, "%d ", logicGate);
                }
            } else {
                int *value = malloc(sizeof(int));
                *value = logicGate;
                push(&stack, value);
            }
        } else {
            length += sprintf(expression+length, "%s ", argv[i]);
            if(argv[i][0] == '-'){
                count++;
            }

        }
    }
    while (!isEmpty(stack) && (logicGate = *((int*)pop(&stack))) != BRACKET_OPEN) {
        length += sprintf(expression+length, "%d ", logicGate);
    }
    printf("expression: %s\n", expression);
    printf("count: %d\n", count);


    filterS *dataFilter = malloc(sizeof(filterS)*count);
    char* token = strtok(expression, " ");
    bool condition = false;
    int i = 0;
    while (token) {
        printf("token: %s\n", token);
        if(token[0] == '-'){
            printf("%d\n", conditionType(token));
            int type = conditionType(token);
            switch (type){
                case NAMES:
                    parseName();
                    break;
                case SIZES:
                    parseSize();
                    break;
                case DATES:
                    parseDate();
                    break;
                case OWNERS:
                    parseOwner();
                    break;
                case PERMS:
                    parsePerm();
                    break;
                default:
                    log("error unknown condition");
            }
        }else{
            printf("error unknown condition");
        }

        while(condition){
            token = strtok(NULL, " ");
        }

    }

    debug("parser end");
}