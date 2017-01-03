/**
 * @file circ_list.c
 * @brief contient toutes les fonctions permettant de g�n�rer une liste circulaire g�n�rique
 *
 *
 * @author Marco Rodrigues Lopes
 * @date avril-mai 2016
 * @version 0.1
*/
#include <stdlib.h>
#include <stdio.h>
#include "circ_list.h"

/**
 * crée une liste
 * @return NULL
 */
circListS* list_create()
{
    return NULL;
}

/**
 * vérifie si une liste est vide
 * @param head tête de la liste
 * @return bool
 */
bool list_empty(circListS* head)
{
    return head == NULL;
}

/**
 * compte le nombre d'�l�ments dans une liste
 * @param head t�te de la liste
 * @return int nombre d'�l�ments
 */
unsigned int list_count(circListS* head)
{
    //si la liste est vide
    if(list_empty(head))
    {
        return 0;
    }
    int count = 0;
    //on parcourt toute la liste
    circListS* temp = head;
    do
    {
        head = list_move(head);
        count++;
    }
    while(head != temp);
    return count;
}

/**
 * se d�place sur le prochain �l�ment
 * @param head t�te de la liste
 * @return Element le prochain �l�ment
 */
circListS* list_move(circListS* head)
{
    return head->next;
}

/**
 * ins�re un �l�ment apr�s la t�te
 * @param head t�te de la liste
 * @param data donn�e � ins�rer
 * @return Element retourne la nouvelle t�te
 */
circListS* list_insert_after(circListS* head, void* data)
{
    circListS* newElement = malloc(sizeof(circListS));
    newElement->content = data;
    //si la liste est vide alors on boucle l'�l�ment sur lui m�me
    if(list_empty(head))
    {
        newElement->next = newElement;
    }
    else
    {
        newElement->next = head->next;
        head->next = newElement;
    }
    return newElement;
}

/**
 * ins�re un �l�ment avant la t�te
 * @param head t�te de la liste
 * @param data donn�e � ins�rer
 * @return Element retourne la nouvelle t�te
 */
circListS* list_insert_before(circListS* head, void* data)
{
    circListS* newElement = malloc(sizeof(circListS));
    newElement->content = data;
    //si la liste est vide alors on boucle l'�l�ment sur lui m�me
    if(list_empty(head))
    {
        newElement->next = newElement;
        return newElement;
    }
    else
    {
        //on met l'�l�ment apr�s head
        newElement->next = head->next;
        head->next = newElement;
        //on inverse les donn�es
        void* temp = head->content;
        head->content = head->next->content;
        head->next->content = temp;
    }
    return head;
}

/**
 * cherche un �l�ment dans la liste
 * @param head t�te de la liste
 * @param data donn�e � chercher
 * @param compare fonction de comparaison des donn�es
 * @return Element retourne l'�l�ment recherch�
 */
circListS* list_search(circListS* head, void* data, int (*compare)(void*, void*))
{
    if(!list_empty(head)){
        circListS* temp = head;
        //on parcourt la liste
        do
        {
            //si on trouve ce qu'on cherche on retourne le r�sultat
            if(compare(head->content, data))
            {
                return head;
            }
            head = list_move(head);
        }
        while(head != temp);
    }
    return NULL;
}

/**
 * supprime un �l�ment dans la liste
 * @param head t�te de la liste
 * @param data donn�e � supprimer
 * @param compare fonction de comparaison des donn�es
 * @return Element retourne l'�l�ment supprim�
 */
circListS* list_remove(circListS* head, void* data, int (*compare)(void*, void*))
{
    if(list_empty(head))
    {
        return NULL;        
    }
    //supprimer le premier �l�ment
    else if(compare(head->content, data))
    {
        //on inverse le premier �l�ment avec le deuxi�me
        void* temp = head->content;
        head->content = head->next->content;
        head->next->content = temp;
        circListS* tempElement = head->next;
        //on saute le deuxi�me �l�ment
        head->next = list_move(list_move(head));
        return tempElement;
    }
    //supprimer dans la liste
    circListS* temp = head;
    do
    {
        //si on trouve data alors on saute l'�l�ment
        if(compare(head->next->content, data))
        {
            head->next = list_move(list_move(head));
            return head;
        }
        head = list_move(head);
    }
    while(head != temp);
    return NULL;
}

/**
 * lib�re la liste
 * @param head t�te de la liste
 * @param data_free fonction lib�rant les donn�es
 * @return Element retourne la t�te
 */
circListS* list_free(circListS* head, void (*data_free)(void*))
{
    if(!list_empty(head)){    
        list_process(head, data_free);
        circListS* tempFree = NULL;
        do
        {
            tempFree = list_move(head);
            head->next = list_move(tempFree);
            free(tempFree);
        }
        while(tempFree != head);
    }
    return NULL;
}

/**
 * applique une action aux donn�es
 * @param head t�te de la liste
 * @param action fonction d�finissant l'action
 * @return void
 */
void list_process(circListS* head, void (*action)(void*))
{
    circListS* temp = head;
    do
    {
        action(head->content);
        head = list_move(head);
    }
    while(head != temp);
}
