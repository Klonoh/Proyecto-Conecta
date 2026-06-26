#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "sugerencias.h"
#include "usuarios.h"
#include "list.h"
#include "extra.h"

static Sugerencia *buscarSugerencia(List *sugerencias, const char *username) {
    Sugerencia *sug = list_first(sugerencias);

    while (sug != NULL) {
        if (strcmp(sug->usuario->user, username) == 0)
            return sug;

        sug = list_next(sugerencias);
    }

    return NULL;
}


static int ordenarSugerencias(const void *a, const void *b) {
    Sugerencia *sugA = *(Sugerencia **)a;
    Sugerencia *sugB = *(Sugerencia **)b;

    return sugB->peso - sugA->peso;
}

void sugerenciasParaTi(Usuario **usuario_actual, Map *usuarios, Graph **grafo, int *sesion_iniciada) {
    if (usuario_actual == NULL || *usuario_actual == NULL) {
        printf("Error: Usuario no válido.\n");
        return;
    }

    limpiarPantalla();
    puts("=======================================");
    puts("        Sugerencias para ti");
    puts("=======================================");

    List *sugerencias = list_create();

    List *nivel1 = getAdjacentLabels(*grafo, (*usuario_actual)->user);
    if (nivel1 == NULL) {
        printf("No hay sugerencias disponibles por ahora.\n");
        list_clean(sugerencias);
        free(sugerencias);
        return;
    }
    char *seguido = list_first(nivel1);

    while (seguido != NULL) {
        List *nivel2 = getAdjacentLabels(*grafo, seguido);
        if (nivel2 == NULL) {
            seguido = list_next(nivel1);
            continue;
        }


        char *candidato = list_first(nivel2);

        while (candidato != NULL) {
            bool soy_yo = strcmp(candidato, (*usuario_actual)->user) == 0;
            bool lo_sigo = yaSigue(*usuario_actual, candidato);

            if (!soy_yo && !lo_sigo) {
                Sugerencia *existente = buscarSugerencia(sugerencias, candidato);
                if (existente != NULL) existente->peso++;
                else {
                    MapPair *pair = map_search(usuarios, candidato);

                    if (pair != NULL) { 
                        Sugerencia *nueva = malloc(sizeof(Sugerencia));
                        nueva->usuario = pair->value;
                        nueva->peso = 1;
                        list_pushBack(sugerencias, nueva);
                    }
                }
            }
            candidato = list_next(nivel2);
        }
        list_clean(nivel2);
        free(nivel2);

        seguido = list_next(nivel1);
    }

    list_clean(nivel1);
    free(nivel1);

    int cantidad = list_size(sugerencias);

    if (cantidad == 0) {
        printf("No hay sugerencias disponibles por ahora.\n");
        list_clean(sugerencias);
        free(sugerencias);
        return;
    }

    Sugerencia **arreglo = malloc(cantidad * sizeof(Sugerencia *));
    Sugerencia *sug = list_first(sugerencias);

    int i = 0;
    while (sug != NULL) {
        arreglo[i++] = sug;
        sug = list_next(sugerencias);
    }

    qsort(arreglo, cantidad, sizeof(Sugerencia *), ordenarSugerencias);

    int limite;

    if(cantidad < 10){
        limite = cantidad;
    }
    else limite = 10;

    for (int i = 0; i < limite; i++) {
        printf("%2d) %s - %d seguidores en común\n", i + 1, arreglo[i]->usuario->user, arreglo[i]->peso);
    }

    puts("=======================================");
    printf("\nIngrese el número del usuario que desea ver (0 para volver): ");

    int opcion;

    while (scanf("%d", &opcion) != 1 || opcion < 0 || opcion > limite) {
        printf("Opción inválida.\n");
        printf("Ingrese nuevamente: ");

        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }

    if (opcion > 0) {
        MostrarPerfil(usuario_actual, arreglo[opcion - 1]->usuario, usuarios, sesion_iniciada, grafo);
    }

    for (int i = 0; i < cantidad; i++) {
        free(arreglo[i]);
    }

    free(arreglo);
    list_clean(sugerencias);
    free(sugerencias);

}

void construirGrafoDesdeUsuarios(Map *usuarios, Graph *grafo) {
    MapPair *pair = map_first(usuarios);

    while (pair != NULL) {
        Usuario *usuario = pair->value;
        addNode(grafo, usuario->user);
        pair = map_next(usuarios);
    }

    pair = map_first(usuarios);

    while (pair != NULL) {
        Usuario *usuario = pair->value;

        Usuario *seguido = list_first(usuario->seguidos);
        while (seguido != NULL) {
            addEdge(grafo, usuario->user, seguido->user);
            seguido = list_next(usuario->seguidos);
        }

        pair = map_next(usuarios);
    }
}

void reconstruirGrafo(Map *usuarios, Graph **grafo) {
    destroyGraph(*grafo);

    *grafo = createGraph();

    construirGrafoDesdeUsuarios(usuarios, *grafo);
}