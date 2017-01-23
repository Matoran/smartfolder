/**
 * \author ISELI Cyril & RODRIGUES Marco
 * \date December 2016 and January 2017
 */

#ifndef SEARCHFOLDER_STACK_H
#define SEARCHFOLDER_STACK_H

#include <stdbool.h>

typedef struct stackIntS{
    int value;
    struct stackIntS *next;
}stackIntS;

extern void createStackInt(stackIntS **stack);
extern void pushInt(stackIntS **stack, int value);
extern int popInt(stackIntS **stack);
extern bool isEmptyInt(stackIntS *stack);
extern int headInt(stackIntS **stack);

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
