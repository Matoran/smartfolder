//
// Created by matoran on 12/28/16.
//

#include <stdlib.h>
#include "stack.h"
#include "logger.h"
#include <stdio.h>

void createStack(stackS **stack){
    *stack = NULL;
}

void push(stackS **stack, void *value){
    stackS *element = malloc(sizeof(stackS));
    element->next = *stack;
    element->value = value;
    *stack = element;
}



void *pop(stackS **stack){
    if(isEmpty(*stack)){
        logFile("stack error: empty stack and pop");
    }else{
        void *nb = (*stack)->value;
        stackS *temp = *stack;
        *stack = (*stack)->next;
        free(temp);
        return nb;
    }
}



bool isEmpty(stackS *stack){
    return stack == NULL;
}

void createStackBool(stackBoolS **stack){
    *stack = NULL;
}

void pushBool(stackBoolS **stack, bool value){
    stackBoolS *element = malloc(sizeof(stackBoolS));
    element->next = *stack;
    element->value = value;
    *stack = element;
}

bool popBool(stackBoolS **stack){
    if(isEmptyBool(*stack)){
        logFile("stack error: empty stack and pop");
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
    while(temp != NULL){
        printf("%d ", temp->value);
        temp = temp->next;
    }
}