#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "list.h"

void add_node(list *l, int data)
{
    if (!l) {
        return; // List not initialized, just reutrn.
    }

    node * new = (node *) malloc (sizeof(node));
    if (!new) {
        perror ("malloc failed\n");
        return; // malloc failed, just return.
    }
    new->data = data;

    pthread_mutex_lock(&l->lock);
    new->next = l->head;
    l->head = new;
    pthread_mutex_unlock(&l->lock);

    return;
}

void delete_node(list *l, int data)
{
    node * temp;
    node * p;
    if (!l || !l->head) {
        return; // List not initialized, just return.
    }

    pthread_mutex_lock(&l->lock);

    if (l->head->data == data) {
        temp = l->head;
        l->head = l->head->next;
        free(temp);
    } else {
        p = l->head;
        while (p->next) {
            if (p->next->data == data) {
                temp = p->next;
                p->next = p->next->next;
                free(temp);
                break;
            }
            p = p->next;
        }
    }

    pthread_mutex_unlock(&l->lock);

    return;
}

void print(list *l)
{
    node *p;
    if (!l) {
        return;
    }

    pthread_mutex_lock(&l->lock);
    p = l->head;
    printf("List :");
    while (p) {
        printf(" %d", p->data);
        p = p->next;
    }
    printf("\n");
    pthread_mutex_unlock(&l->lock);
}

void list_init (list *p)
{
    p->head = NULL;
    pthread_mutex_init(&p->lock, NULL);
}
