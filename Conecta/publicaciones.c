#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "publicaciones.h"

#include "TDAs/extra.h"

static int ordenar(const void *a, const void *b) {
    Publicacion *pubA = *(Publicacion **)a;
    Publicacion *pubB = *(Publicacion **)b;

    //orden descendente (mas reciente primero)
    if (pubB->timestamp > pubA->timestamp) return 1; 

    if (pubB->timestamp < pubA->timestamp) return -1;

    return 0;
}

void publicarMensaje(Usuario *usuario_actual) {
    limpiarPantalla();
    puts("=======================================");
    puts("           Publicar Mensaje");
    puts("=======================================");
    char contenido[141];
    printf("Ingrese el contenido de su mensaje (máximo 140 caracteres): ");

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    fgets(contenido, sizeof(contenido), stdin);
    contenido[strcspn(contenido, "\n")] = '\0';

    Publicacion *nueva_publicacion = (Publicacion *)malloc(sizeof(Publicacion));
    strcpy(nueva_publicacion->contenido, contenido);
    strcpy(nueva_publicacion->autor, usuario_actual->user);
    nueva_publicacion->timestamp = time(NULL);

    list_pushBack(usuario_actual->publicaciones, nueva_publicacion);

    // Notificar a los seguidores
    List *seguidores = usuario_actual->seguidores;
    Usuario *seguidor = list_first(seguidores);
    while (seguidor != NULL) {
        char notificacion[200];
        snprintf(notificacion, sizeof(notificacion), "El usuario %s ha publicado un nuevo mensaje.", usuario_actual->user);
        queue_insert(seguidor->notificaciones, strdup(notificacion));
        seguidor = list_next(seguidores);
    }
}

void verFeed(Usuario* usuario_actual) {
    if (usuario_actual == NULL) {
        printf("Error: Usuario no válido.\n");
        return;
    }
    puts("=======================================");
    puts("                Mi Feed");
    puts("=======================================");

    //primero se consiguen el numero total de publicaciones del feed
    int nPublicaciones = 0;
    Usuario* aux = list_first(usuario_actual->seguidos);
    while (aux != NULL) {
        nPublicaciones += list_size(aux->publicaciones);
        aux = list_next(usuario_actual->seguidos);
    }

    if (nPublicaciones == 0) {
        printf("No hay publicaciones en tu feed.\n");
        return;
    }
    
    //se crea un arreglo dinámico para almacenar todas las publicaciones de los usuarios seguidos
    Publicacion** publicaciones = (Publicacion**) malloc(nPublicaciones * sizeof(Publicacion*));
    int i = 0;
    aux = list_first(usuario_actual->seguidos);
    while (aux != NULL) {
        Publicacion* pub = list_first(aux->publicaciones);
        while (pub != NULL) {
            publicaciones[i] = pub;
            i++;
            pub = list_next(aux->publicaciones);
        }
        aux = list_next(usuario_actual->seguidos);
    }

    //se ordena el arreglo de publicaciones por fecha (de mas reciente a mas antigua)
    qsort(publicaciones, nPublicaciones, sizeof(Publicacion*), ordenar);

    //se muestran las publicaciones 
    Publicacion* pub = NULL;
    for (int i = 0; i < nPublicaciones; i++) {
        pub = publicaciones[i];
        char fecha[30];
        formatearFecha(pub->timestamp, fecha, sizeof(fecha));
        printf("\n%s:\n\n%s\n\n%s\n\n", pub->autor, pub->contenido, fecha);
        if (i < nPublicaciones - 1) {
            puts("=======================================");
        }
    }

    //Liberar memoria del arreglo de publicaciones
    free(publicaciones);

}   

void verNotificaciones(Usuario *usuario_actual) {
    if (usuario_actual == NULL) {
        printf("Error: Usuario no válido.\n");
        return;
    }
    limpiarPantalla();
    puts("=======================================");
    puts("           Notificaciones");
    puts("=======================================");
    Queue *notificaciones = usuario_actual->notificaciones;
    char *notificacion = queue_front(notificaciones);

    if (notificacion == NULL) {
        printf("No tienes notificaciones pendientes.\n");
        return;
    }

    while (notificacion != NULL) {
        printf("• %s\n", notificacion);
        notificacion = queue_next(notificaciones);
    }
    char *mensaje = queue_front(notificaciones);

    while (mensaje != NULL) {
        free(mensaje);
        mensaje = queue_next(notificaciones);
    }
    queue_clean(notificaciones); // Limpiar las notificaciones después de mostrarlas
}
