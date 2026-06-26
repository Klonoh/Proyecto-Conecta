#ifndef ENDTIDADES_H
#define ENDTIDADES_H

#include <time.h>
#include "list.h"
#include "queue.h"

typedef struct Usuario {
    char user[16];
    char pass[21];
    List *publicaciones;
    List *seguidores;
    List *seguidos;
    Queue *notificaciones;
} Usuario;

typedef struct Publicacion {
    char contenido[141];
    char autor[16];
    time_t timestamp;
} Publicacion;

typedef struct Sugerencia {
    Usuario *usuario;
    int peso;
} Sugerencia;

typedef struct Tendencia {
    char hashtag[50];
    int contador;
} Tendencia;

#endif