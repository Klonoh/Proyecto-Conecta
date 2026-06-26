#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "guardado.h"
#include "entidades.h"
#include "usuarios.h"

void leerArchivo(Map *usuarios, FILE *archivo) {
    char line[200];

    //primera pasada:
    //se crean los usuarios y se agregan sus publicaciones y notificaciones
    while (fgets(line, sizeof(line), archivo)) {
        char username[16], password[21];
        int leidos = sscanf(line, "USUARIO %s %s", username, password);
        if (leidos != 2) continue; // línea vacía o basura, saltarla
    
        inicializarUsuario(usuarios, username, password);
        MapPair *pair = map_search(usuarios, username);
        Usuario *usuario = pair->value;

        fgets(line, sizeof(line), archivo); //leer la línea de publicaciones
        int n;
        sscanf(line, "PUBLICACIONES %d", &n);

        for (int i = 0; i < n; i++) {
            fgets(line, sizeof(line), archivo);
            char autor[16], contenido[141];
            time_t timestamp;
            sscanf(line, "%s %ld %[^\n]", autor, &timestamp, contenido);
            
            Publicacion *nueva_publicacion = (Publicacion *)malloc(sizeof(Publicacion));
            strcpy(nueva_publicacion->autor, autor);
            strcpy(nueva_publicacion->contenido, contenido);
            nueva_publicacion->timestamp = timestamp;
            list_pushBack(usuario->publicaciones, nueva_publicacion);
        }

        fgets(line, sizeof(line), archivo); //leer la línea de seguidos
        sscanf(line, "SEGUIDOS %d", &n);

        for (int i = 0; i < n; i++) {
            fgets(line, sizeof(line), archivo); //se saltan los seguidos
        }
        
        fgets(line, sizeof(line), archivo); //leer la línea de seguidores
        sscanf(line, "SEGUIDORES %d", &n);

        for (int i = 0; i < n; i++) {
            fgets(line, sizeof(line), archivo); //se saltan los seguidores
        }

        fgets(line, sizeof(line), archivo); //leer la línea de notificaciones
        sscanf(line, "NOTIFICACIONES %d", &n);

        for (int i = 0; i < n; i++) {
            fgets(line, sizeof(line), archivo);
            line[strcspn(line, "\n")] = '\0'; // quitar el salto de línea del final
            queue_insert(usuario->notificaciones, strdup(line)); //se ingresan notificaciones
        }    

        fgets(line, sizeof(line), archivo); //leer la línea de fin
    }


    //segunda pasada: como los usuarios ya están creados, se agregan los seguidos y seguidores
    rewind(archivo);
    while(fgets(line, sizeof(line), archivo)) {
        char username[16], password[21];
        int leidos = sscanf(line, "USUARIO %s %s", username, password);
        if (leidos != 2) continue; // línea vacía o basura, saltarla

        MapPair *pair_usuario = map_search(usuarios, username);

        if (pair_usuario == NULL) {
            continue;
        }

        Usuario *usuario = pair_usuario->value;

        fgets(line, sizeof(line), archivo); //leer la línea de publicaciones
        int n;
        sscanf(line, "PUBLICACIONES %d", &n);

        for (int i = 0; i < n; i++) {
            fgets(line, sizeof(line), archivo); //se saltan las publicaciones
        }
        
        fgets(line, sizeof(line), archivo); //leer la línea de seguidos
        sscanf(line, "SEGUIDOS %d", &n);

        for (int i = 0; i < n; i++) {
            fgets(line, sizeof(line), archivo);
            char seguido[16];
            sscanf(line, "%s", seguido);
            MapPair *pair_seguido = map_search(usuarios, seguido);
            if (pair_seguido != NULL) {
                Usuario *usuario_seguido = pair_seguido->value;
                list_pushBack(usuario->seguidos, usuario_seguido);
            }
        }
        
        fgets(line, sizeof(line), archivo); //leer la línea de seguidores
        sscanf(line, "SEGUIDORES %d", &n);

        for (int i = 0; i < n; i++) {
            fgets(line, sizeof(line), archivo);
            char seguidor[16];
            sscanf(line, "%s", seguidor);
            MapPair *pair_seguidor = map_search(usuarios, seguidor);
            if (pair_seguidor != NULL) {
                Usuario *usuario_seguidor = pair_seguidor->value;
                list_pushBack(usuario->seguidores, usuario_seguidor);
            }
        }

        fgets(line, sizeof(line), archivo); //leer la línea de notificaciones
        sscanf(line, "NOTIFICACIONES %d", &n);

        for (int i = 0; i < n; i++) {
            fgets(line, sizeof(line), archivo); //se saltan las notificaciones
        }

        fgets(line, sizeof(line), archivo); //leer la línea de fin
    }
}

void salir(Map *usuarios, FILE *archivo) {
    if(archivo != NULL){
        fclose(archivo);
    }

    archivo = fopen("Usuarios.txt", "w");

    if (archivo == NULL) {
        printf("Error al guardar los usuarios.\n");
        exit(1);
    }

    MapPair *pair = map_first(usuarios);


    while(pair != NULL) {
        Usuario *aux = pair->value;
        fprintf(archivo, "USUARIO %s %s\n", aux->user, aux->pass); //imprimir user y pass

        //formato de salida general: "nombre + cantidad", luego en cada siguiente linea se imprime el contenido de cada elemento
        //ej: PUBLICACIONES 2
        // publicacion1
        // publicacion2
        // y asi sucesivamente para seguidos, seguidores y notificaciones

        fprintf(archivo, "PUBLICACIONES %d\n", list_size(aux->publicaciones)); //publicaciones
        Publicacion *aux_pub = list_first(aux->publicaciones);  
        while(aux_pub != NULL) {
            fprintf(archivo, "%s %ld %s\n", aux_pub->autor, (long)aux_pub->timestamp, aux_pub->contenido); //autor + fecha + contenido
            aux_pub = list_next(aux->publicaciones);
        }

        fprintf(archivo, "SEGUIDOS %d\n", list_size(aux->seguidos)); 
        Usuario *aux_seguidos = list_first(aux->seguidos);
        while(aux_seguidos != NULL) {
            fprintf(archivo, "%s\n", aux_seguidos->user);
            aux_seguidos = list_next(aux->seguidos);
        }

        fprintf(archivo, "SEGUIDORES %d\n", list_size(aux->seguidores)); 
        Usuario *aux_seguidores = list_first(aux->seguidores);
        while(aux_seguidores != NULL) {
            fprintf(archivo, "%s\n", aux_seguidores->user);
            aux_seguidores = list_next(aux->seguidores);
        }

        fprintf(archivo, "NOTIFICACIONES %d\n", queue_size(aux->notificaciones)); 
        char *aux_notificaciones = queue_front(aux->notificaciones);
        while(aux_notificaciones != NULL) {
            fprintf(archivo, "%s\n", aux_notificaciones);
            aux_notificaciones = queue_next(aux->notificaciones);
        }
        fprintf(archivo, "FIN\n");
        pair = map_next(usuarios);
    }
    fclose(archivo);
    exit(0);
}

