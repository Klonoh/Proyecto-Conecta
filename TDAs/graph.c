#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "list.h"
#include "map.h"

/* =========================================
 *         ESTRUCTURAS INTERNAS
 * ========================================= */
struct Graph {
    Map* adjacencyMap;
};

int is_equal_string(void *key1, void *key2) {
    return strcmp((char*)key1, (char*)key2) == 0;
}

/* =========================================
 *         IMPLEMENTACIÓN
 * ========================================= */
Graph* createGraph() {
    Graph* nuevo = malloc(sizeof(Graph));
    if (nuevo == NULL) exit(EXIT_FAILURE);
    nuevo->adjacencyMap = map_create(is_equal_string);
    return nuevo;
}

void addNode(Graph* g, const char* label) {
    if (!g || !label) return;
    if (map_search(g->adjacencyMap, (void *) label) != NULL) return;
    char* copiaLabel = strdup(label);
    List* lista = list_create();
    map_insert(g->adjacencyMap, copiaLabel, lista);
}

void removeNode(Graph* g, const char* label) {
    if (!g || !label) return;

    //Eliminar todas las aristas que apuntan a este nodo en otros nodos
    MapPair* pair = map_first(g->adjacencyMap);
    while (pair != NULL) {
        List* edges = (List*) pair->value;
        Edge* e = list_first(edges);
        while (e != NULL) {
            if (is_equal_string(e->target, (void*) label)) {
                list_popCurrent(edges);
                free(e->target);
                free(e);
                break;
            }
            e = list_next(edges);
        }
        pair = map_next(g->adjacencyMap);
    }

    // 2. Eliminar la lista de aristas del nodo y el nodo del mapa
    MapPair* par = map_remove(g->adjacencyMap, (void*) label);
    if (par == NULL) return;

    List* edgesList = (List*) par->value;
    Edge* e = list_first(edgesList);
    while (e != NULL) {
        free(e->target);
        free(e);
        e = list_next(edgesList);
    }
    list_clean(edgesList);
    free(edgesList);
    free(par->key);
    free(par);
}

void addEdge(Graph* g, const char* src, const char* dest) {
    if (!g || !src || !dest) return;
    if (map_search(g->adjacencyMap, (void *) dest) == NULL) return;
    MapPair* par = map_search(g->adjacencyMap, (void *) src);
    if (par == NULL) return;

    // Verificar que la arista no exista ya
    List* edges = (List*) par->value;
    Edge* e = list_first(edges);
    while (e != NULL) {
        if (is_equal_string(e->target, (void*) dest)) return;
        e = list_next(edges);
    }

    Edge* nuevo = malloc(sizeof(Edge));
    if (nuevo == NULL) exit(EXIT_FAILURE);
    nuevo->target = strdup(dest);
    list_pushBack(par->value, nuevo);
}

void removeEdge(Graph* g, const char* src, const char* dest) {
    if (!g || !src || !dest) return;
    MapPair* par = map_search(g->adjacencyMap, (void *) src);
    if (par == NULL) return;

    List* edges = (List*) par->value;
    Edge* e = list_first(edges);
    while (e != NULL) {
        if (is_equal_string(e->target, (void*) dest)) {
            list_popCurrent(edges);
            free(e->target);
            free(e);
            return;
        }
        e = list_next(edges);
    }
}

List* getEdges(Graph* g, const char* label) {
    if (!g || !label) return NULL;
    MapPair* pair = map_search(g->adjacencyMap, (void *) label);
    return (pair == NULL) ? NULL : pair->value;
}

List* getAdjacentLabels(Graph* g, const char* label) {
    if (!g || !label) return NULL;
    MapPair* par = map_search(g->adjacencyMap, (void *) label);
    if (par == NULL) return NULL;

    List* aristas = par->value;
    List* nueva = list_create();
    Edge* e = list_first(aristas);
    while (e != NULL) {
        list_pushBack(nueva, e->target);
        e = list_next(aristas);
    }
    return nueva;
}

void destroyGraph(Graph* g) {
    if (!g) return;
    MapPair* pair = map_first(g->adjacencyMap);
    while (pair != NULL) {
        List* edgesList = (List*) pair->value;
        Edge* e = list_first(edgesList);
        while (e != NULL) {
            free(e->target);
            free(e);
            e = list_next(edgesList);
        }
        list_clean(edgesList);
        free(edgesList);
        free(pair->key);
        pair = map_next(g->adjacencyMap);
    }
    map_clean(g->adjacencyMap);
    free(g->adjacencyMap);
    free(g);
}