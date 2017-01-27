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

/**
 * add int to the stack
 * @param stack
 * @param value
 */
void pushInt(stackIntS **stack, int value) {
    stackIntS *element = mallocw(sizeof(stackIntS));
    element->next = *stack;
    element->value = value;
    *stack = element;
}


/**
 * return the head of the stack and remove it
 * @param stack
 * @return int stack head
 */
int popInt(stackIntS **stack) {
    if (isEmptyInt(*stack)) {
        logger("stack error: empty stack and pop\n", ERROR, true);
        exit(EXIT_FAILURE);
    } else {
        int nb = (*stack)->value;
        stackIntS *temp = *stack;
        *stack = (*stack)->next;
        free(temp);
        return nb;
    }
}

/**
 * give the value of the head
 * @param stack
 * @return stack head
 */
int headInt(stackIntS **stack) {
    if (isEmptyInt(*stack)) {
        logger("stack error: empty stack and pop\n", ERROR, true);
        exit(EXIT_FAILURE);
    } else {
        return (*stack)->value;
    }
}

/**
 * check if the stack is empty
 * @param stack
 * @return true or false
 */
bool isEmptyInt(stackIntS *stack) {
    return stack == NULL;
}

/**
 * add bool to the stack
 * @param stack
 * @param value
 */
void pushBool(stackBoolS **stack, bool value) {
    stackBoolS *element = mallocw(sizeof(stackBoolS));
    element->next = *stack;
    element->value = value;
    *stack = element;
}

/**
 * return the head of the stack and remove it
 * @param stack
 * @return bool stack head
 */
bool popBool(stackBoolS **stack) {
    if (isEmptyBool(*stack)) {
        logger("stack error: empty stack and pop\n", ERROR, true);
        exit(EXIT_FAILURE);
    } else {
        bool boolean = (*stack)->value;
        stackBoolS *temp = *stack;
        *stack = (*stack)->next;
        free(temp);
        return boolean;
    }
}

/**
 * check if the stack is empty
 * @param stack
 * @return true or false
 */
bool isEmptyBool(stackBoolS *stack) {
    return stack == NULL;
}

/**
 * display the stack
 * @param stack
 */
void displayStackBool(stackBoolS *stack) {
    stackBoolS *temp = stack;
    logger("stack{", DEBUG, false);
    while (temp != NULL) {
        logger("%d ", DEBUG, false, temp->value);
        temp = temp->next;
    }
    logger("}\n", DEBUG, false);
}
