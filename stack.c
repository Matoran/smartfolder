/**
 * @author ISELI Cyril & RODRIGUES Marco
 * @brief Usable stack of type int or bool
 * @version 0.1
 * @date December 2016 and January 2017
 * @file stack.c
 *
 * A simple stack with push pop isEmpty head with 2 types int or bool
 */
#include "stack.h"
#include "logger.h"
#include "wrappersyscall.h"

void pushInt(stackIntS **stack, int value){
    stackIntS *element = mallocw(sizeof(stackIntS));
    element->next = *stack;
    element->value = value;
    *stack = element;
}



int popInt(stackIntS **stack){
    if(isEmptyInt(*stack)){
        logger("stack error: empty stack and pop\n", ERROR, true);
    }else{
        int nb = (*stack)->value;
        stackIntS *temp = *stack;
        *stack = (*stack)->next;
        free(temp);
        return nb;
    }
}

int headInt(stackIntS **stack){
    if(isEmptyInt(*stack)){
        logger("stack error: empty stack and pop\n", ERROR, true);
    }else{
        return (*stack)->value;
    }
}



bool isEmptyInt(stackIntS *stack){
    return stack == NULL;
}

void pushBool(stackBoolS **stack, bool value){
    stackBoolS *element = mallocw(sizeof(stackBoolS));
    element->next = *stack;
    element->value = value;
    *stack = element;
}

bool popBool(stackBoolS **stack){
    if(isEmptyBool(*stack)){
        logger("stack error: empty stack and pop\n", ERROR, true);
    }else{
        bool boolean = (*stack)->value;
        stackBoolS *temp = *stack;
        *stack = (*stack)->next;
        free(temp);
        return boolean;
    }
}

bool isEmptyBool(stackBoolS *stack){
    return stack == NULL;
}

void displayStackBool(stackBoolS *stack){
    stackBoolS *temp = stack;
    logger("stack{", DEBUG, false);
    while(temp != NULL){
        logger("%d ", DEBUG, false, temp->value);
        temp = temp->next;
    }
    logger("}\n", DEBUG, false);
}