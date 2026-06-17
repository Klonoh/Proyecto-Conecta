#ifndef GRAPH_H
#define GRAPH_H
#include "list.h"

typedef struct Graph Graph;

typedef struct {
    char* target;
} Edge;

Graph* createGraph();

void destroyGraph(Graph* g);

void addNode(Graph* g, const char* label);

void removeNode(Graph* g, const char* label);

void addEdge(Graph* g, const char* src, const char* dest);

void removeEdge(Graph* g, const char* src, const char* dest);

List* getEdges(Graph* g, const char* label);

List* getAdjacentLabels(Graph* g, const char* label);

#endif