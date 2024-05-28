/*
    File Name: list.c
    Name: Nicolas Villegas-Kirchman
    Data: 12/2/23
    Description: Contains functions for creating a linked list with an array in each node and functions for adding first, adding last, removing first, removing last, numItems, destroying the list, setting an item at a specific index, getting an item at a specific index, getting first, and getting last.
*/

#include "list.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct list { //O(1) Structure design of list that contains a head node of struct node, a count variable and a compare function. Compare function is provided by the user. A struct list named LIST is declared here as well.
    int count;
    struct node *head;
} LIST;

typedef struct node { //O(1) Declares NODE of type struct node. Contains a void ** for storing an array of data, ints for first value index, size (num of elements in array), and array length. Has node pointers to next and prev node.
    void **data;
    struct node *next;
    struct node *prev;
    int first;
    int size;
    int length;
} NODE;

LIST *createList() { //O(1) Creates a linked list and creates a dummy node with everything initialized to 0 since its a dummy node.
    struct list *lp;
    lp = malloc(sizeof(LIST));
    assert(lp != NULL);
    lp->count = 0;
    lp->head = malloc(sizeof(NODE));
    assert(lp->head != NULL);
    lp->head->next = lp->head;
    lp->head->prev = lp->head;
    lp->head->first = 0;
    lp->head->length = 0;
    lp->head->data = malloc(sizeof(void *) * lp->head->length);
    assert(lp->head->data != NULL);
    lp->head->size = 0;
    return lp;
}

void destroyList(LIST *lp) //O(n) Iterates through linked list and destroys the array in the node and then the node.
{
    NODE *pDel, *pNext;
    assert(lp != NULL);
    pDel = lp->head;
    do {
        pNext = pDel->next;
        free(pDel->data);
        free(pDel);
        pDel = pNext;
    } while (pDel!= lp->head);
    free(lp);
}


int numItems(LIST *lp) {// O(1) Checks if lp is null. Returns lp->count as count is incremented when a value is added.
    assert(lp != NULL);
    return lp->count;
}


void addFirst(LIST *lp, void *item) { //O(1) Creates a new node if array in node is full. Sets item to the index right before the previous first value in the first node. Modulo used to prevent array out of bounds.
    assert(lp != NULL && item != NULL);
    if (lp->head->next->size == lp->head->next->length) {
        NODE *newNode = malloc(sizeof(NODE));
        assert(newNode != NULL);
        newNode->prev = lp->head;
        newNode->next = lp->head->next;
        newNode->prev->next = newNode;
        lp->head->next = newNode;
        newNode->length = newNode->next->length * 2;
        if (newNode->length == 0)
            newNode->length = 10;
        newNode->data = malloc(sizeof(void *) * lp->head->next->length);
        assert(newNode->data != NULL);
        newNode->first = 0;
        newNode->size = 0;
    }
    int index = (lp->head->next->first - 1 + lp->head->next->length) % lp->head->next->length;
    lp->head->next->data[index] = item;
    lp->head->next->first = index;
    lp->count++;
    lp->head->next->size++;
}


void addLast(LIST *lp, void *item) { //O(1) Creates a new node if the array in the node is full. Sets item to 1 after the previous last index in the array of the last node. Modulo used to prevent array out of bounds.
    assert(lp != NULL && item != NULL);
    if (lp->head->prev->size == lp->head->prev->length) {
        NODE *newNode = malloc(sizeof(NODE));
        assert(newNode != NULL);
        newNode->next = lp->head;
        newNode->prev = lp->head->prev;
        newNode->prev->next = newNode;
        lp->head->prev = newNode;
        newNode->length = newNode->prev->length * 2;
        if (newNode->length == 0)
            newNode->length = 10;
        newNode->data = malloc(sizeof(void *) * lp->head->prev->length);
        assert(newNode->data != NULL);
        newNode->first = 0;
        newNode->size = 0;
    }
    lp->head->prev->data[(lp->head->prev->first + lp->head->prev->size) % lp->head->prev->length] = item;
    lp->count++;
    lp->head->prev->size++;
}

void *removeFirst(LIST *lp) { //O(1) Removes the first value in the first node and if the node's new size is 0, it deletes the node.
    assert(lp != NULL && lp->count > 0);
    void *item = lp->head->next->data[lp->head->next->first];
    lp->head->next->first = (lp->head->next->first + 1) % lp->head->next->length;
    lp->head->next->size--;
    lp->count--;
    if (lp->head->next->size == 0) {
        NODE *pDel = lp->head->next;
        lp->head->next = pDel->next;
        pDel->next->prev = lp->head;
        free(pDel->data);
        free(pDel);
    }
    return item;
}


void *removeLast(LIST *lp) { //O(1) Removes the last value in the last node nad if the node's new size is 0, it deletes the node.
    assert(lp != NULL && lp->count > 0);
    void *item = lp->head->prev->data[(lp->head->prev->first + lp->head->prev->size - 1) % lp->head->prev->length];
    lp->head->prev->size--;
    lp->count--;
    if (lp->head->prev->size == 0) {
        NODE *pDel = lp->head->prev;
        lp->head->prev = pDel->prev;
        pDel->prev->next = lp->head;
        free(pDel->data);
        free(pDel);
    }
    return item;
}

void *getFirst(LIST *lp) { //O(1) Gets the first value in the first node.
    assert(lp != NULL && lp->count > 0);
    return lp->head->next->data[lp->head->next->first];
}

void *getLast(LIST *lp) { //O(1) Gets the last value in the last node.
    assert(lp != NULL && lp->count > 0);
    return lp->head->prev->data[(lp->head->prev->first + lp->head->prev->size - 1) % lp->head->prev->length];
}

void setItem(LIST *lp, int index, void *item) { //O(n) (Amount of nodes in list) Decrements index by the length of each array in each node starting from the first node. Once index is less than the size of the array in that node, sets the value at that index to item.
    assert(lp != NULL && index < lp->count);
    assert(item != NULL);

    NODE *current = lp->head->next;
    int currentIndex = index;
    while (currentIndex >= current->size) {
        currentIndex -= current->size;
        current = current->next;
    }

    current->data[(current->first + currentIndex) % current->length] = item;
}


void *getItem(LIST *lp, int index) { //O(n) (Amount of nodes in list) Decrements index by the length of each array in each node starting from the first node. Once index is less than the size of the array in that node, returns the value at that index.
    assert(lp != NULL);
    assert(index < lp->count);

    NODE *current = lp->head->next;
    int currentIndex = index;
    while (currentIndex >= current->size) {
        currentIndex -= current->size;
        current = current->next;
    }

    return current->data[currentIndex % current->length];
}




