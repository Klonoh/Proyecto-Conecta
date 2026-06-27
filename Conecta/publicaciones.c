#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "publicaciones.h"

#include "extra.h"

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

    while (1){
        printf("Ingrese el contenido de su mensaje (máximo 140 caracteres):\n> "); 

        fgets(contenido, sizeof(contenido), stdin);
        int longitud = strlen(contenido);

        //si el mensaje tiene salto de linea al final, significa q tiene menos de 140 caracteres, por lo tanto es valido
        if (longitud > 1 && contenido[longitud - 1] == '\n') { 
            contenido[longitud - 1] = '\0'; //eliminar salto de linea
            break;
        } else {
            printf("El contenido debe tener entre 1 y 140 caracteres.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF); // limpiar buffer de entrada para eliminar cualquier exceso de caracteres
            continue;
        }

        if (strlen(contenido) == 0) {
            printf("El contenido no puede estar vacío.\n");
            continue;
        }
        break;
    }

    //se crea la publicacion 
    Publicacion *nueva_publicacion = (Publicacion *)malloc(sizeof(Publicacion));
    strcpy(nueva_publicacion->contenido, contenido);
    strcpy(nueva_publicacion->autor, usuario_actual->user);
    nueva_publicacion->timestamp = time(NULL);

    list_pushFront(usuario_actual->publicaciones, nueva_publicacion);

    // Notificar a los seguidores
    List *seguidores = usuario_actual->seguidores;
    Usuario *seguidor = list_first(seguidores);
    while (seguidor != NULL) {
        char notificacion[200];
        snprintf(notificacion, sizeof(notificacion), "El usuario %s ha publicado un nuevo mensaje.", usuario_actual->user);
        queue_insert(seguidor->notificaciones, strdup(notificacion));
        seguidor = list_next(seguidores);
    }
    puts("Mensaje publicado exitosamente.");
    presioneTeclaParaContinuar();
}

void verFeed(Usuario* usuario_actual) {
    if (usuario_actual == NULL) {
        printf("Error: Usuario no válido.\n");
        presioneTeclaParaContinuar();
        return;
    }
    limpiarPantalla();
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
        presioneTeclaParaContinuar();
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
    int mostradas = 0;
    int opcion = 0;
    do {
        int lim = mostradas + 3;
        if (lim > nPublicaciones) lim = nPublicaciones;

        for (int i = mostradas; i < lim; i++) {
            Publicacion *pub = publicaciones[i];
            char fecha[30];
            formatearFecha(pub->timestamp, fecha, sizeof(fecha));
            printf("\n%s:\n\n%s\n\n%s\n\n", pub->autor, pub->contenido, fecha);
            if (i < nPublicaciones - 1) {
                puts("=======================================");
            }
        } 
        mostradas = lim;

        printf("\033[s");//guarda el cursor en esta posicion

        int offset = 0;
        if (mostradas < nPublicaciones) {
            printf("\n1) Ver más publicaciones (mostrando %d de %d)\n", mostradas, nPublicaciones);
            offset = 1;
        }

        if (mostradas == nPublicaciones) printf("Mostrando %d de %d publicaciones.\n", mostradas, mostradas);
        printf("%d) Volver al menú principal\n", offset + 1);
        printf("\nIngrese su opción: ");

        while (scanf("%d", &opcion) != 1 || opcion < 1 || opcion > (1 + offset)) {
            printf("Opción inválida. Intente de nuevo: ");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);//limpiar buffer
        }

        int c;
        while ((c = getchar()) != '\n' && c != EOF); //limpiar buffer por si quedan caracteres despues
        
        if (offset == 1 && opcion == 1) {
            printf("\033[u\033[J"); //borra desde la posicion del cursor hasta la actual
            continue; 
        }
        break;
    
    } while (mostradas < nPublicaciones);

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
        presioneTeclaParaContinuar();
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
    presioneTeclaParaContinuar();
}
