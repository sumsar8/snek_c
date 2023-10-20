#ifndef LINKEDLIST_H
#define LINKEDLIST_H

struct node{
    int x;
    int y;
    struct node *next;
}; 

void push(struct node** head, int dirx, int diry);
void pop(struct node* head);
void pushatend(struct node* head);

#endif


