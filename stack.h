/**
 * \author ISELI Cyril & RODRIGUES Marco
 * \date December 2016 and January 2017
 */

#ifndef SEARCHFOLDER_STACK_H
#define SEARCHFOLDER_STACK_H

#include <stdbool.h>

typedef struct stackS{
    void *value;
    struct stackS *next;
}stackS;

extern void createStack(stackS **stack);
extern void push(stackS **stack, void *value);
extern void *pop(stackS **stack);
extern bool isEmpty(stackS *stack);

typedef struct stackBoolS{
    bool value;
    struct stackBoolS *next;
}stackBoolS;
extern void createStackBool(stackBoolS **stack);
extern void pushBool(stackBoolS **stack, bool value);
extern bool popBool(stackBoolS **stack);
extern bool isEmptyBool(stackBoolS *stack);
extern void displayStackBool(stackBoolS *stack);

#endif //SEARCHFOLDER_STACK_H
