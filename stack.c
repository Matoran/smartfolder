//
// Created by matoran on 12/28/16.
//

#include <stdlib.h>
#include "stack.h"
#include "logger.h"

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