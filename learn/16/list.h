#ifndef __LIST_H__
#define __LIST_H__

#include <pthread.h>

typedef struct node
{
    int data;
    struct node * next;
} node;

typedef struct list
{
    node * head;
    pthread_mutex_t lock;
} list;

void list_init (list *p);
void add_node(list *l, int data);
void delete_node(list *l, int data);
void print(list *l);

#endif //__LIST_H__
