/**
 * @author ISELI Cyril & RODRIGUES Marco
 * @brief Usable stack of type int or bool
 * @version 0.1
 * @date December 2016 and January 2017
 * @file stack.h
 *
 * A simple stack with push pop isEmpty head with 2 types int or bool
 */

#ifndef SEARCHFOLDER_STACK_H
#define SEARCHFOLDER_STACK_H

#include <stdbool.h>

/**
 * Simple stack that can contain ints
 */
typedef struct stackIntS{
    int value;
    struct stackIntS *next;
}stackIntS;
extern void pushInt(stackIntS **stack, int value);
extern int popInt(stackIntS **stack);
extern bool isEmptyInt(stackIntS *stack);
extern int headInt(stackIntS **stack);

/**
 * Simple stack that can contain bools
 */
typedef struct stackBoolS{
    bool value;
    struct stackBoolS *next;
}stackBoolS;
extern void pushBool(stackBoolS **stack, bool value);
extern bool popBool(stackBoolS **stack);
extern bool isEmptyBool(stackBoolS *stack);
extern void displayStackBool(stackBoolS *stack);

#endif //SEARCHFOLDER_STACK_H
