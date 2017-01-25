/**
 * @author ISELI Cyril & RODRIGUES Marco
 * @brief Parse the programs args
 * @version 0.1
 * @date December 2016 and January 2017
 * @file parser.c
 *
 * Parse args, test if the directories are valid, set the linker destination
 * and parse infixe to postfix expression
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

/**
 * check if the string is quoted by %%
 * @param string
 * @return true or false
 */
static bool isQuoted(const char *string) {
    if (string[0] == '%' && string[strlen(string) - 1] == '%')
        return true;
    return false;
}

/**
 * convert int to the associated string
 * @param val
 * @return char* readable for humans
 */
static char *getLogicGateName(int val) {
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

/**
 * check if the path is a directory
 * @param path
 * @return int positive if true, 0 if false
 */
static int isDirectory(const char *path) {
    struct stat statbuf;
    statw(path, &statbuf);
    return S_ISDIR(statbuf.st_mode);
}

/**
 * test if the string is a logic gate then return the type of the logic gate
 * @param string
 * @return return int value defined in the header or -1 if not a gate
 */
static int isLogicGate(const char *string) {
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

/**
 * test if the directory is writeable
 * @param fpath
 * @return bool
 */
static bool isWriteable(char *fpath) {
    struct stat bufstat;
    statw(fpath, &bufstat);
    if (bufstat.st_mode & S_IWUSR) {
        return true;
    }
    return false;
}

/**
 * test if the directory is readable
 * @param fpath
 * @return bool
 */
static bool isReadable(char *fpath) {
    struct stat bufstat;
    statw(fpath, &bufstat);
    if (bufstat.st_mode & S_IRUSR) {
        return true;
    }
    return false;
}

/**
 * test if the string is a condition then return the type
 * @param string
 * @return int defined in the header or -1 if nota condition type
 */
static int conditionType(const char *string) {
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

/**
 * create nameS, parse the expression and fill nameS
 * @return filled nameS
 */
static void *parseName() {
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

/**
 * create sizeS, parse the expression and fill sizeS
 * @return filled sizeS
 */
static void *parseSize() {
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

/**
 * create dateS, parse the expression and fill dateS
 * @return filled dateS
 */
static void *parseDate() {
    dateS *date = mallocw(sizeof(dateS));

    //type
    char *token = strtok(NULL, " .");
    if (strcmp(token, "status") == 0) {
        date->type = STATUS;
        token = strtok(NULL, " .");
    } else if (strcmp(token, "modified") == 0) {
        date->type = MODIFIED;
        token = strtok(NULL, " .");
    } else if (strcmp(token, "accessed") == 0) {
        date->type = ACCESSED;
        token = strtok(NULL, " .");
    } else {
        date->type = ACCESSED;
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

    //TODO test date correctly
    //date
    struct tm dateargs = {0};
    dateargs.tm_mday = atoi(token);
    dateargs.tm_mon = atoi(strtok(NULL, ".")) - 1;
    dateargs.tm_year = atoi(strtok(NULL, " .")) - 1900;
    date->timestamp = mktime(&dateargs);
    return date;
}

/**
 * create ownerS, parse the expression and fill ownerS
 * @return filled ownerS
 */
static void *parseOwner() {
    ownerS *owner = mallocw(sizeof(ownerS));

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

/**
 * create permS, parse the expression and fill permS
 * @return filled permS
 */
static void *parsePerm() {
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

/**
 * parse received args, convert infix to postfix expression,
 * create simplified expression and create filter conditions
 * @param argc number of arguments
 * @param argv array of string
 */
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
    stackIntS *stack = NULL;
    int logicGate;
    //simulate bracket open
    pushInt(&stack, BRACKET_OPEN);
    char *expression = mallocw(sizeof(char) * (argc - 3) * 255);
    expression[0] = '\0';
    int length = 0;
    int countTotal = 0, countConditions = 0;
    for (int i = 3; i < argc; ++i) {
        if ((logicGate = isLogicGate(argv[i])) >= 0) {
            logger("%s is a logic gate\n", DEBUG, true, argv[i]);
            if (logicGate == BRACKET_CLOSE) {
                logger("%s is a bracket close\n", DEBUG, true, argv[i]);
                while (!isEmptyInt(stack) && (logicGate = popInt(&stack)) != BRACKET_OPEN) {
                    logger("pop logic gate %s\n", DEBUG, true, getLogicGateName(logicGate));
                    length += sprintf(expression + length, "%d ", logicGate);
                    countTotal++;
                }
                logger("actual expression %s\n", DEBUG, true, expression);
            } else {
                while (logicGate != NOT && logicGate != BRACKET_OPEN && !isEmptyInt(stack) && headInt(&stack) == NOT) {
                    logger("pop logic gate %s\n", DEBUG, true, getLogicGateName(headInt(&stack)));
                    length += sprintf(expression + length, "%d ", popInt(&stack));
                    countTotal++;
                }
                logger("%s push in stack\n", DEBUG, true, argv[i]);
                pushInt(&stack, logicGate);
            }
        } else {
            logger("%s is not a logic gate\n", DEBUG, true, argv[i]);
            if (isQuoted(argv[i])) {
                length += sprintf(expression + length, "%s ", &argv[i][1]);
                strcpy(&expression[length - 2], " ");
                logger("is quoted new expression : %s \n", DEBUG, true, expression);
            } else {
                length += sprintf(expression + length, "%s ", argv[i]);
            }
            logger("actual expression %s\n", DEBUG, true, expression);
            if (argv[i][0] == '-' && strlen(argv[i]) > 1) {
                countTotal++;
                countConditions++;
            }

        }

    }
    //simulate bracket close expression is surrounded by bracket open and bracket close
    while (!isEmptyInt(stack) && (logicGate = popInt(&stack)) != BRACKET_OPEN) {
        length += sprintf(expression + length, "%d ", logicGate);
        countTotal++;
    }
    logger("actual expression \"%s\", count conditions %d, count total %d\n", DEBUG, true, expression, countConditions,
           countTotal);
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