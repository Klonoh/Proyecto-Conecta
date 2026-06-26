#ifndef QUEUE_H
#define QUEUE_H
#include "list.h"

typedef List Queue;

static Queue *queue_create() {return list_create();}

static void queue_insert(Queue *queue, void *data) {list_pushBack(queue, data);}

static void *queue_remove(Queue *queue) {return list_popFront(queue);}

static void *queue_front(Queue *queue) {return list_first(queue);}

//Operacion especial para iterar sobre la cola
static void *queue_next(Queue *queue) {return list_next(queue);}

static void queue_clean(Queue *queue) {list_clean(queue);}

static int queue_size(Queue *queue) {return list_size(queue);}

#endif /* QUEUE_H */