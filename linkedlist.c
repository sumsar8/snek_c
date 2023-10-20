#include "linkedlist.h"
#include <stdlib.h>

void push(struct node** head, int dirx, int diry){
    struct node* newnode = (struct node*)malloc(sizeof(struct node));

    newnode->next = *head;
    newnode->x = (*head)->x + 50 * dirx;
    newnode->y = (*head)->y + 50 * diry;
    *head = newnode;
}
void pushatend(struct node* head) {
    struct node* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = (struct node *) malloc(sizeof(struct node));
    current->next->x = head->x + 50;
    current->next->y = head->y + 50;
    current->next->next = NULL;
}
void pop(struct node* head){
    struct node* current = head;
    if (head->next == NULL) {
        free(head);
        return;
    }
    while(current->next->next != NULL){
        current = current->next;
    }
    free(current->next->next);
    current->next = NULL;
}