/**
 * @file circ_list.h
 * @brief contient la structure de données de la liste
 *
 *
 * @author Marco Rodrigues Lopes
 * @date avril-mai 2016
 * @version 0.1
*/
#ifndef CIRC_LIST
#define CIRC_LIST

#include <stdbool.h>

typedef struct circListS {
    void* content;
    struct circListS* next;
} circListS;

circListS* list_create();
bool list_empty(circListS* head);
unsigned int list_count(circListS* head);
circListS* list_move(circListS* head);
circListS* list_insert_after(circListS* head, void* data);
circListS* list_insert_before(circListS* head, void* data);
circListS* list_search(circListS* head, void* data, int (*compare)(void*, void*));
circListS* list_remove(circListS* head, void* data, int (*compare)(void*, void*));
circListS* list_free(circListS* head, void (*data_free)(void*));
void list_process(circListS* head, void (*action)(void*));
#endif // CIRC_LIST
