//
// Created by matoran on 12/28/16.
//

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

#endif //SEARCHFOLDER_STACK_H
