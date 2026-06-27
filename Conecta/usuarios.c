#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "usuarios.h"
#include "publicaciones.h"
#include "sugerencias.h"
#include "generales.h"
#include "extra.h"

void inicializarUsuario(Map *usuarios, char *username, char *password) {
    Usuario *nuevo_usuario = (Usuario *)malloc(sizeof(Usuario));
    strcpy(nuevo_usuario->user, username);
    strcpy(nuevo_usuario->pass, password);
    nuevo_usuario->publicaciones = list_create();
    nuevo_usuario->seguidores = list_create();
    nuevo_usuario->seguidos = list_create();
    nuevo_usuario->notificaciones = queue_create();
    map_insert(usuarios, strdup(username), nuevo_usuario);
}

void buscarUsuario(Map *usuarios, Usuario **usuario_actual, int *sesion_iniciada, Graph **grafo) {
    while (1) {
        limpiarPantalla();
        puts("=======================================");
        puts("           Buscar Usuario");
        puts("=======================================");
        char username[16];
        printf("Ingrese el nombre de usuario a buscar (0 para volver): ");
        scanf("%15s", username);

        int c;
        while ((c = getchar()) != '\n' && c != EOF); //limpiar buffer
        if (strcmp(username, "0") == 0) return;

        convertirUsernameMinusculas(username);
        MapPair *pair = map_first(usuarios);
        int coincidencias = 0;

        puts("\nResultados de la búsqueda:");

        while (pair != NULL) {
            Usuario *usuario = pair->value;

            if (strstr(usuario->user, username) != NULL) {
                coincidencias++;
                printf("%d) %s\n", coincidencias, usuario->user);
            }

            pair = map_next(usuarios);
        }

        if (coincidencias > 0) {
            int opcion;
            printf("\nIngrese el número del perfil que desea ver (0 para cancelar): ");

            while (scanf("%d", &opcion) != 1) {
                printf("Opción inválida. Intente de nuevo: \n");
                while (getchar() != '\n'); // Limpiar el buffer de entrada
            }

            int c;
            while ((c = getchar()) != '\n' && c != EOF); //limpiar buffer
            
            if (opcion > 0 && opcion <= coincidencias) {
                pair = map_first(usuarios);
                int contador = 1;

                while (pair != NULL) {
                    Usuario *usuario = pair->value;

                    if (strstr(usuario->user, username) != NULL) {
                        if (contador == opcion) {
                            MostrarPerfil(usuario_actual, usuario, usuarios, sesion_iniciada, grafo);
                            break;
                        }
                        contador++;
                    }

                    pair = map_next(usuarios);
                }
            } else if (opcion == 0) {
                printf("Operación cancelada.\n");
                return;
            } else {
                printf("Opción inválida.\n");
                presioneTeclaParaContinuar();
                continue;
            }
        } 
        else if (coincidencias == 0) {
            printf("No se encontraron usuarios que coincidan con la búsqueda.\n");
            presioneTeclaParaContinuar();
            return;
        }
    }
}

int mostrarPublicacionesPaginadas(List *publicaciones, Publicacion **pub)
{
    int mostradas = 0;

    while (*pub != NULL && mostradas < 3)
    {
        char fecha[30];
        formatearFecha((*pub)->timestamp, fecha, sizeof(fecha));

        printf("\n%s:\n\n%s\n\n%s\n\n",
               (*pub)->autor,
               (*pub)->contenido,
               fecha);

        puts("=======================================");

        *pub = list_next(publicaciones);
        mostradas++;
    }

    return (*pub != NULL);
}

void MostrarPerfil(Usuario **usuario_actual, Usuario *usuario, Map *usuarios, int *sesion_iniciada, Graph **grafo) {


    if (usuario_actual != NULL && *usuario_actual != NULL && strcmp((*usuario_actual)->user, usuario->user) != 0) {
        List *seguidos = (*usuario_actual)->seguidos;
        Usuario *temp = list_first(seguidos);
        bool ya_sigue = false;

        while (temp != NULL) { //verificar si el usuario actual ya sigue al usuario cuyo perfil se está mostrando
            if (strcmp(temp->user, usuario->user) == 0) {
                ya_sigue = true;
                break;
            }
            temp = list_next(seguidos);
        }
        
        Publicacion *pub_otro = list_first(usuario->publicaciones);
        int quedan_publicaciones = 1;
        int opcion;

        do {
            limpiarPantalla();
            puts("=======================================");
            printf("               %s\n", usuario->user);
            puts("=======================================");
            printf("Publicaciones: %d | Seguidores: %d | Seguidos: %d", list_size(usuario->publicaciones), list_size(usuario->seguidores), list_size(usuario->seguidos));
            puts("\n=======================================");

            if (ya_sigue == true) printf("\nSiguiendo\n");
            else printf("\nNo le sigues\n");
    
            puts("=======================================");

            // Mostrar el bloque actual de 3 publicaciones
            quedan_publicaciones = mostrarPublicacionesPaginadas(usuario->publicaciones, &pub_otro);
            
            int offset = 0;
            if (quedan_publicaciones) {
                printf("\n1) Ver más publicaciones\n");
                offset = 1;
            }
            
            if (ya_sigue == true) {
                printf("\n%d) Dejar de seguir\n", offset + 1);
            } else {
                printf("\n%d) Seguir\n", offset + 1);
            }
            printf("\n%d) Volver\n", offset + 2);
            int opcion;
            printf("\nIngrese su opción: ");
            
            while (scanf("%d", &opcion) != 1 || opcion < 1 || opcion > (2 + offset)) {
                printf("Opción inválida. Intente de nuevo: ");
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
            }
            
            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            //si el usuario eligió ver mas
            if (offset == 1 && opcion == 1) {
                continue; //el do-while da otra vuelta y muestra las siguientes 3
            }

            int accion = (offset == 1) ? opcion - 1 : opcion;//ajustar accion segun offset

            if (accion == 1) {
                if (ya_sigue == true) {
                    dejarDeSeguirUsuario(*usuario_actual, usuario, *grafo);
                    ya_sigue = false; //actualizar estado
                } else if (ya_sigue == false) {
                    seguirUsuario(*usuario_actual, usuario, *grafo);
                    ya_sigue = true; //actualizar estado
                }
                presioneTeclaParaContinuar();
                pub_otro = list_first(usuario->publicaciones);
                break;
            } else if (accion == 2) {
                return; // Volver 
            } 
        } while (1);
    } 

    else if (usuario_actual != NULL && *usuario_actual != NULL && strcmp((*usuario_actual)->user, usuario->user) == 0) {

        Publicacion *pub = list_first(usuario->publicaciones);
        int quedanPublicaciones = 1;
        int opcion;

        do {
            limpiarPantalla();
            puts("=======================================");
            printf("               %s\n", usuario->user);
            puts("=======================================");
            printf("Publicaciones: %d | Seguidores: %d | Seguidos: %d", list_size(usuario->publicaciones), list_size(usuario->seguidores), list_size(usuario->seguidos));
            puts("\n=======================================");

            quedanPublicaciones = mostrarPublicacionesPaginadas(usuario->publicaciones, &pub);

            int offset = 0;
            if (quedanPublicaciones) {
                printf("\n1) Ver más publicaciones\n");
                offset = 1; 
            }
            printf("%d) Ver seguidores\n", offset + 1);
            printf("%d) Ver seguidos\n", offset + 2);
            printf("%d) Editar perfil\n", offset + 3);
            printf("%d) Volver\n", offset + 4);
            printf("\nIngrese su opción: ");

            while (scanf("%d", &opcion) != 1 || opcion < 1 || opcion > 4 + offset) {
                printf("Opción inválida. Ingrese nuevamente: ");
                int c;
                while ((c = getchar()) != '\n' && c != EOF); //limpiar buffer entrada
            }
            
            if (offset == 1 && opcion == 1) continue; //el usuario eligio mostrar mas publis

            int accion = (offset == 1) ? opcion - 1 : opcion; //ajustar accion segun offset

            switch (accion) {
                case 1:
                    verListaUsuarios(usuario_actual, usuario->seguidores, "Seguidores", "No tienes seguidores.", usuarios, sesion_iniciada, grafo);
                    pub = list_first(usuario->publicaciones); //reiniciar puntero a publicaciones 
                    break;
                case 2:
                    verListaUsuarios(usuario_actual, usuario->seguidos, "Seguidos", "No estás siguiendo a nadie.", usuarios, sesion_iniciada, grafo);
                    pub = list_first(usuario->publicaciones); //reiniciar puntero a publicaciones
                    break;
                case 3:
                    editarPerfil(usuario_actual, usuarios, sesion_iniciada, grafo);
                    pub = list_first(usuario->publicaciones); //reiniciar puntero a publicaciones
                    break;
                case 4:
                    return; // Volver
            }
        } while (1);
    } 
    else {
        printf("Error: Usuario no válido.\n");
    }
}

void seguirUsuario(Usuario *usuario_actual, Usuario *usuario_a_seguir, Graph *grafo) {
    if (usuario_actual == NULL || usuario_a_seguir == NULL) {
        printf("Error: Usuario no válido.\n");
        return;
    }

    if (strcmp(usuario_actual->user, usuario_a_seguir->user) == 0) {
        printf("No puedes seguirte a ti mismo.\n");
        return;
    }
    List *seguidos = usuario_actual->seguidos;
    Usuario *temp = list_first(seguidos);
    while (temp != NULL) {
        if (strcmp(temp->user, usuario_a_seguir->user) == 0) {
            printf("Ya sigues a %s.\n", usuario_a_seguir->user);
            return;
        }
        temp = list_next(seguidos);
    }
    list_pushBack(usuario_actual->seguidos, usuario_a_seguir);
    list_pushBack(usuario_a_seguir->seguidores, usuario_actual);
    char notificacion[100];
    snprintf(notificacion, sizeof(notificacion), "El usuario %s ha comenzado a seguirte.", usuario_actual->user);
    queue_insert(usuario_a_seguir->notificaciones, strdup(notificacion));
    addEdge(grafo, usuario_actual->user, usuario_a_seguir->user);
    printf("Ahora sigues a %s.\n", usuario_a_seguir->user);
}

void dejarDeSeguirUsuario(Usuario *usuario_actual, Usuario *usuario_a_dejar_de_seguir, Graph *grafo) {
    if (usuario_actual == NULL || usuario_a_dejar_de_seguir == NULL) {
        printf("Error: Usuario no válido.\n");
        return;
    }
    List *seguidos = usuario_actual->seguidos;
    Usuario *temp = list_first(seguidos);
    while (temp != NULL) {
        if (strcmp(temp->user, usuario_a_dejar_de_seguir->user) == 0) {
            list_popCurrent(seguidos);
            break;
        }
        temp = list_next(seguidos);
    }


    List *seguidores = usuario_a_dejar_de_seguir->seguidores;
    Usuario *aux = list_first(seguidores);
    while (aux != NULL) {
        if (strcmp(aux->user, usuario_actual->user) == 0) {
            list_popCurrent(seguidores);
            break;
        }
        aux = list_next(seguidores);
    }

    removeEdge(grafo, usuario_actual->user, usuario_a_dejar_de_seguir->user);

    printf("Has dejado de seguir a %s.\n", usuario_a_dejar_de_seguir->user);
}

void verListaUsuarios( Usuario **usuario_actual, List *lista, const char *titulo, const char *mensaje_vacio, Map *usuarios, int *sesion_iniciada, Graph **grafo) {
    if (usuario_actual == NULL) {
        printf("Error: Usuario no válido.\n");
        return;
    }
    limpiarPantalla();
    puts("=======================================");
    printf("              %s\n", titulo);
    puts("=======================================");
    Usuario *usuario = list_first(lista);

    if (usuario == NULL) {
        printf("%s\n", mensaje_vacio);
        return;
    }

    int contador = 0;
    while (usuario != NULL) {
        contador++;
        printf("%2d) %s\n", contador, usuario->user);
        usuario = list_next(lista);
    }
    puts("=======================================");

    printf("\nIngrese el número del usuario que desea ver (0 para volver): ");
    int opcion;
    while (scanf("%d", &opcion) != 1) {
        printf("Opción inválida. Intente de nuevo: \n");
        while (getchar() != '\n'); // Limpiar el buffer de entrada
    }
    while (opcion < 0 || opcion > contador) {
        printf("Opción inválida. Intente de nuevo: ");

        while (scanf("%d", &opcion) != 1) {
            printf("Opción inválida. Intente de nuevo: ");

            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    if (opcion == 0) {
        return; // Cancelar y volver 
    }
    usuario = list_first(lista);
    int contador_usuario = 1;
    while (usuario != NULL) {
        if (contador_usuario == opcion) {
            MostrarPerfil(usuario_actual, usuario, usuarios, sesion_iniciada, grafo);
            return;
        }
        usuario = list_next(lista);
        contador_usuario++;
    }
}

void editarPerfil(Usuario **usuario_actual, Map *usuarios, int *sesion_iniciada, Graph **grafo) {
    if (usuario_actual == NULL || *usuario_actual == NULL) {
        printf("Error: Usuario no válido.\n");
        return;
    }
    limpiarPantalla();
    puts("=======================================");
    puts("           Editar Perfil");
    puts("=======================================");
    puts("1) Cambiar nombre de usuario");
    puts("2) Cambiar contraseña");
    puts("3) Eliminar cuenta");
    puts("4) Volver");

    int opcion;
    printf("\nIngrese su opción: ");
    while (scanf("%d", &opcion) != 1 || opcion < 1 || opcion > 4) {
        printf("Opción inválida.\n");
        printf("Ingrese nuevamente: ");

        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    if(opcion == 1) {
        char nuevo_username[16];
        char nuevo_username_input[50];

        while (1) {
            printf("Ingrese el nuevo nombre de usuario: ");
            scanf("%49s", nuevo_username_input);

            if (!usernameValido(nuevo_username_input)) {
                printf("Nombre de usuario inválido (4 a 15 caracteres, letras sin tilde, números, \".\" y \"_\"):\n");
                continue;
            }

            convertirUsernameMinusculas(nuevo_username_input);

            if (strcmp(nuevo_username_input, (*usuario_actual)->user) == 0) {
                printf("El nuevo nombre de usuario no puede ser igual al actual.\n");
                continue;
            }

            if (map_search(usuarios, nuevo_username_input) != NULL) {
                printf("El nombre de usuario ya existe. Intente con otro.\n");
                continue;
            }

            strcpy(nuevo_username, nuevo_username_input);
            break; 
        }

            
            char nombre_antiguo[16];
            strcpy(nombre_antiguo, (*usuario_actual)->user);

            Usuario *usuario_sacado = map_remove(usuarios, nombre_antiguo);

            if (usuario_sacado == NULL) {
                printf("Error al actualizar el usuario.\n");
                return;
            }

            strcpy((*usuario_actual)->user, nuevo_username);

            Publicacion *pub = list_first((*usuario_actual)->publicaciones);
                while (pub != NULL) {
                strcpy(pub->autor, nuevo_username);
                pub = list_next((*usuario_actual)->publicaciones);
            }

            map_insert(usuarios, strdup(nuevo_username), *usuario_actual);

            reconstruirGrafo(usuarios, grafo);

            printf("Nombre de usuario actualizado correctamente.\n");
            presioneTeclaParaContinuar();

    }
    else if(opcion == 2) {
        char password_actual[21];
        char nueva_password[21];
        char confirmar_password[21];

        while (1) {
            printf("Ingrese su contraseña actual: ");
            scanf("%20s", password_actual);

            if (strcmp(password_actual, (*usuario_actual)->pass) == 0) break;

            printf("Contraseña incorrecta. Intente nuevamente.\n");
        }
        
        while (1) {
            printf("Ingrese la nueva contraseña: ");
            scanf("%20s", nueva_password);

            if (strcmp(nueva_password, (*usuario_actual)->pass) == 0) {
                printf("La nueva contraseña no puede ser igual a la actual.\n");
            } 
            else break;
        }

        while (1) {
            printf("Confirme la nueva contraseña: ");
            scanf("%20s", confirmar_password);

            if (strcmp(nueva_password, confirmar_password) == 0) break;

            printf("Las contraseñas no coinciden. Intente nuevamente.\n");
        }

        strcpy((*usuario_actual)->pass, nueva_password);

        printf("Contraseña actualizada exitosamente.\n");
        presioneTeclaParaContinuar();
    }
    else if(opcion == 3) {
        char confirmacion[10];

        printf("Esta acción eliminará permanentemente su cuenta.\n");
        printf("Escriba \"confirmo\" para continuar: ");

        scanf("%9s", confirmacion);

        if(strcmp(confirmacion, "confirmo") != 0){
            printf("Texto incorrecto.\n");
            editarPerfil(usuario_actual, usuarios, sesion_iniciada, grafo);
            return;
        }

        Usuario *seguido = list_first((*usuario_actual)->seguidos);

        while (seguido != NULL) {
            Usuario *aux = list_first(seguido->seguidores);
            while (aux != NULL) {
                if (aux == *usuario_actual) {
                    list_popCurrent(seguido->seguidores);
                    break;
                }
                aux = list_next(seguido->seguidores);
            }
            seguido = list_next((*usuario_actual)->seguidos);
        }

        Usuario *seguidor = list_first((*usuario_actual)->seguidores);

        while (seguidor != NULL) {

            Usuario *temp = list_first(seguidor->seguidos);

            while (temp != NULL) {
                if (temp == *usuario_actual) {
                    list_popCurrent(seguidor->seguidos);
                    break;
                }
                temp = list_next(seguidor->seguidos);
            }

            seguidor = list_next((*usuario_actual)->seguidores);
        }

        Usuario *usuario_removido = map_remove(usuarios, (*usuario_actual)->user);

        if (usuario_removido == NULL) {
            printf("Error al eliminar la cuenta.\n");
            return;
        }

        Publicacion *pub = list_first((*usuario_actual)->publicaciones);
        while (pub != NULL) {
            free(pub);
            pub = list_next((*usuario_actual)->publicaciones);
        }
        list_clean((*usuario_actual)->publicaciones);
        
        char *mensaje = queue_front((*usuario_actual)->notificaciones);

        while (mensaje != NULL) {   
            free(mensaje);
            mensaje = queue_next((*usuario_actual)->notificaciones);
        }

        queue_clean((*usuario_actual)->notificaciones);

        list_clean((*usuario_actual)->seguidores);
        list_clean((*usuario_actual)->seguidos);

        free((*usuario_actual)->publicaciones);
        free((*usuario_actual)->seguidores);
        free((*usuario_actual)->seguidos);
        free((*usuario_actual)->notificaciones);

        removeNode(*grafo, (*usuario_actual)->user);

        free(*usuario_actual);

        *usuario_actual = NULL;
        *sesion_iniciada = 0;

        return;

    }
}

bool yaSigue(Usuario *usuario_actual, const char *username) {
    Usuario *seguido = list_first(usuario_actual->seguidos);

    while (seguido != NULL) {
        if (strcmp(seguido->user, username) == 0)
            return true;

        seguido = list_next(usuario_actual->seguidos);
    }

    return false;
}