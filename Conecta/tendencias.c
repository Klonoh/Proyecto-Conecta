#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tendencias.h"
#include "entidades.h"
#include "generales.h"
#include "list.h"
#include "extra.h"

static void contarHashtag(Map *hashtags, const char *hashtag) {
    MapPair *pair = map_search(hashtags, (void *) hashtag);

    if (pair != NULL) {
        int *contador = pair->value;
        (*contador)++;
    } 
    else {
        int *contador = malloc(sizeof(int));

        if (contador == NULL) {
            printf("Error al reservar memoria para contador de hashtag.\n");
            return;
        }

        *contador = 1;

        map_insert(hashtags, strdup(hashtag), contador);
    }
}

static void procesarHashtagsPublicacion(Map *hashtags, const char *contenido) {
    int i = 0;

    while (contenido[i] != '\0') {
        if (contenido[i] == '#') {
            char hashtag[50];
            int k = 0;

            hashtag[k++] = '#';
            i++;

            while (contenido[i] != '\0' && hashtagValido(contenido[i])) {
                if (k < 49) {
                    hashtag[k++] = contenido[i];
                }

                i++;
            }

            hashtag[k] = '\0';

            if (k > 1) {
                convertirUsernameMinusculas(hashtag);
                contarHashtag(hashtags, hashtag);
            }
        } 
        else {
            i++;
        }
    }
}
int ordenarTendencias(const void *a, const void *b) {
    Tendencia *tendenciaA = (Tendencia *)a;
    Tendencia *tendenciaB = (Tendencia *)b;

    return tendenciaB->contador - tendenciaA->contador;
}
static void liberarMapaConteos(Map *hashtags) {
    MapPair *pair = map_first(hashtags);

    while (pair != NULL) {
        char key[50];
        strcpy(key, (char *)pair->key);

        int *contador = map_remove(hashtags, key);
        free(contador);

        pair = map_first(hashtags);
    }

    map_clean(hashtags);
    free(hashtags);
}
static bool publicacionContieneHashtag(const char *contenido, const char *hashtag) {
    int i = 0;

    while (contenido[i] != '\0') {
        if (contenido[i] == '#') {
            char hashtag_encontrado[50];
            int k = 0;

            hashtag_encontrado[k++] = '#';
            i++;

            while (contenido[i] != '\0' && hashtagValido(contenido[i])) {
                if (k < 49) {
                    hashtag_encontrado[k++] = contenido[i];
                }

                i++;
            }

            hashtag_encontrado[k] = '\0';

            if (k > 1) {
                convertirUsernameMinusculas(hashtag_encontrado);

                if (strcmp(hashtag_encontrado, hashtag) == 0) {
                    return true;
                }
            }
        } 
        else {
            i++;
        }
    }

    return false;
}

static void mostrarPublicacionesPorHashtag(Map *usuarios, const char *hashtag) {
    List *publicaciones_hashtag = list_create();

    MapPair *pair = map_first(usuarios);

    while (pair != NULL) {
        Usuario *usuario = pair->value;

        Publicacion *pub = list_first(usuario->publicaciones);

        while (pub != NULL) {
            if (publicacionContieneHashtag(pub->contenido, hashtag)) {
                list_pushBack(publicaciones_hashtag, pub);
            }

            pub = list_next(usuario->publicaciones);
        }

        pair = map_next(usuarios);
    }

    int total = list_size(publicaciones_hashtag);

    if (total == 0) {
        printf("No hay publicaciones para %s.\n", hashtag);
        list_clean(publicaciones_hashtag);
        free(publicaciones_hashtag);
        return;
    }

    printf("\n=======================================\n");
    printf("        Publicaciones con %s\n", hashtag);
    printf("=======================================\n");

    Publicacion *pub = list_first(publicaciones_hashtag);
    int mostradas = 0;

    while (pub != NULL) {
        int contador_lote = 0;

        while (pub != NULL && contador_lote < 5) {
            char fecha[30];
            formatearFecha(pub->timestamp, fecha, sizeof(fecha));

            printf("\n%s:\n\n%s\n\n%s\n", pub->autor, pub->contenido, fecha);
            puts("=======================================");

            mostradas++;
            contador_lote++;

            pub = list_next(publicaciones_hashtag);
        }

        if (pub == NULL) {
            printf("\nNo hay más publicaciones para mostrar.\n");
            break;
        }

        printf("\033[s");//guarda el cursor en esta posicion

        int opcion;
        printf("\nMostrando %d de %d publicaciones.\n", mostradas, total);
        printf("1) Ver más\n");
        printf("2) Salir\n");
        printf("Ingrese su opción: ");

        while (scanf("%d", &opcion) != 1 || opcion < 1 || opcion > 2) {
            printf("Opción inválida. Intente nuevamente: ");

            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }

        if (opcion == 2) {
            break;
        } else if (opcion == 1) {
            printf("\033[u\033[J"); //borra desde la posicion del cursor hasta la actual
            continue; 
        }
    }

    list_clean(publicaciones_hashtag);
    free(publicaciones_hashtag);
}


void verTendencias(Map *usuarios) {
    limpiarPantalla();
    puts("=======================================");
    puts("              Tendencias");
    puts("=======================================");

    Map *hashtags = map_create(is_equal_str);

    MapPair *pair = map_first(usuarios);

    while (pair != NULL) {
        Usuario *usuario = pair->value;

        Publicacion *pub = list_first(usuario->publicaciones);

        while (pub != NULL) {
            procesarHashtagsPublicacion(hashtags, pub->contenido);
            pub = list_next(usuario->publicaciones);
        }

        pair = map_next(usuarios);
    }

    int cantidad = 0;
    MapPair *pair_hashtag = map_first(hashtags);

    while (pair_hashtag != NULL) {
        cantidad++;
        pair_hashtag = map_next(hashtags);
    }

    if (cantidad == 0) {
        printf("No hay hashtags publicados todavía.\n");
        liberarMapaConteos(hashtags);
        return;
    }

    Tendencia *tendencias = malloc(cantidad * sizeof(Tendencia));

    if (tendencias == NULL) {
        printf("Error al reservar memoria para tendencias.\n");
        liberarMapaConteos(hashtags);
        return;
    }

    pair_hashtag = map_first(hashtags);
    int i = 0;

    while (pair_hashtag != NULL) {
        strcpy(tendencias[i].hashtag, (char *)pair_hashtag->key);
        tendencias[i].contador = *((int *)pair_hashtag->value);

        i++;
        pair_hashtag = map_next(hashtags);
    }

    qsort(tendencias, cantidad, sizeof(Tendencia), ordenarTendencias);

    int limite;

    if (cantidad < 10) {
        limite = cantidad;
    } else {
        limite = 10;
    }

    for (int i = 0; i < limite; i++) {
        printf("%2d) %s - %d publicaciones\n", i + 1, tendencias[i].hashtag, tendencias[i].contador);
    }

    puts("=======================================");
    printf("\nIngrese el número del hashtag que desea ver (0 para volver): ");

    int opcion;

    while (scanf("%d", &opcion) != 1 || opcion < 0 || opcion > limite) {
        printf("Opción inválida. Intente nuevamente: ");

        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    int c;
    while ((c = getchar()) != '\n' && c != EOF); //limpiar buffer
    if (opcion > 0) {
        mostrarPublicacionesPorHashtag(usuarios, tendencias[opcion - 1].hashtag);
    }

    free(tendencias);
    liberarMapaConteos(hashtags);
}