# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <string.h>
# include <stdbool.h>
# include <ctype.h>
# include <time.h>
# include "TDAs/graph.h"
# include "TDAs/list.h"
# include "TDAs/map.h"
# include "TDAs/queue.h"
# include "TDAs/extra.h"

typedef struct Usuario {
    char user[16];
    char pass[21];
    List *publicaciones; 
    List *seguidores;
    List *seguidos;
    Queue* notificaciones;
} Usuario;

typedef struct Publicacion {
    char contenido[141];
    char autor[16];
    time_t timestamp;
} Publicacion;

bool iniciarSesion(Map *usuarios, Usuario **usuario_actual);
bool registrarUsuario(Map *usuarios, Usuario **usuario_actual);
void inicializarUsuario(Map *usuarios, char *username, char *password);
void leerArchivo(Map *usuarios, FILE *archivo);
void salir(Map *usuarios, FILE *archivo);
void mostrarMenuInicial(void);
void mostrarMenuPrincipal(void);

int is_equal_str(void *key1, void *key2){
  return strcmp((char *)key1, (char *)key2) == 0;
}

bool iniciarSesion(Map *usuarios, Usuario **usuario_actual) {
    limpiarPantalla();
    puts("========================================");
    puts("     Iniciar Sesión");
    puts("========================================");
    char username[16];
    char password[21];
    printf("Ingrese su nombre de usuario: ");
    scanf("%s", username);
    for (int i = 0; username[i] != '\0'; i++) {
        username[i] = tolower(username[i]);
    }
    printf("Ingrese su contraseña: ");
    scanf("%s", password);
    MapPair *pair = map_search(usuarios, username);
    if (pair == NULL) {
        printf("El nombre de usuario no existe. Intente nuevamente.\n");
        return 0;
    }
    Usuario *usuario = pair->value;
    if (strcmp(usuario->pass, password) != 0) {
        printf("Contraseña incorrecta. Intente nuevamente.\n");
        return 0;
    }
    *usuario_actual = usuario;
    return 1;
}

bool registrarUsuario(Map *usuarios, Usuario **usuario_actual) {
    limpiarPantalla();
    puts("========================================");
    puts("     Registrar Usuario");
    puts("========================================");
    char username[16];
    char password[21];
    printf("Ingrese un nombre de usuario: (máximo 15 caracteres) ");
    scanf("%s", username);
    for (int i = 0; username[i] != '\0'; i++) {
        username[i] = tolower(username[i]);
    }
    printf("Ingrese una contraseña: (máximo 20 caracteres) ");
    scanf("%s", password);
    if (map_search(usuarios, username) != NULL) {
        printf("El nombre de usuario ya existe. Intente con otro.\n");
        return 0;
    }

    inicializarUsuario(usuarios, username, password);
    printf("Usuario registrado exitosamente.\n");
    
    *usuario_actual = map_search(usuarios, username)->value;
    return 1;
}

void inicializarUsuario(Map *usuarios, char *username, char *password) {
    Usuario *nuevo_usuario = (Usuario *)malloc(sizeof(Usuario));
    strcpy(nuevo_usuario->user, username);
    strcpy(nuevo_usuario->pass, password);
    nuevo_usuario->publicaciones = list_create();
    nuevo_usuario->seguidores = list_create();
    nuevo_usuario->seguidos = list_create();
    nuevo_usuario->notificaciones = queue_create();
    map_insert(usuarios, username, nuevo_usuario);
}

void leerArchivo(Map *usuarios, FILE *archivo) {
    char line[200];
    while (fgets(line, sizeof(line), archivo) && line[0] != '\0') {
        char username[16], password[21];
        sscanf(line, "%s %s", username, password);
        inicializarUsuario(usuarios, username, password);
        while (fgets(line, sizeof(line), archivo) && line[0] != ']') {
            if (line[0] == 'P') {
                while (fgets(line, sizeof(line), archivo) && line[0] != ']') {
                    char autor[16], contenido[141];
                    time_t timestamp;
                    sscanf(line, "%s %ld %[^\n]", autor, &timestamp, contenido);
                    Publicacion *nueva_publicacion = (Publicacion *)malloc(sizeof(Publicacion));
                    strcpy(nueva_publicacion->autor, autor);
                    strcpy(nueva_publicacion->contenido, contenido);
                    nueva_publicacion->timestamp = timestamp;
                    MapPair *pair = map_search(usuarios, username);
                    if (pair != NULL) {
                        Usuario *usuario = pair->value;
                        list_pushBack(usuario->publicaciones, nueva_publicacion);
                    }   
                }
            } 
            else if (line[0] == 'T') {
                char seguido[16];
                while (fgets(line, sizeof(line), archivo) && line[0] != ']') {
                    sscanf(line, "%s", seguido);
                }
            }
            else if (line[0] == 'S') {
                char seguidor[16];
                while (fgets(line, sizeof(line), archivo) && line[0] != ']') {
                    sscanf(line, "%s", seguidor);
                }
            }
            else if (line[0] == 'N') {
                char notificacion[200];
                while (fgets(line, sizeof(line), archivo) && line[0] != ']') {
                    sscanf(line, "%[^\n]", notificacion);
                    MapPair *pair = map_search(usuarios, username);
                    if (pair != NULL) {
                        Usuario *usuario = pair->value;
                        queue_insert(usuario->notificaciones, strdup(notificacion));
                    }
                }
            
            }
            
        }
    }
    rewind(archivo);
    while(fgets(line, sizeof(line), archivo) && line[0] != '\0') {
        char username[16], password[21];
        sscanf(line, "%s %s", username, password);
        while (fgets(line, sizeof(line), archivo) && line[0] != ']') {
            if (line[0] == 'P') {
                while (fgets(line, sizeof(line), archivo) && line[0] != ']') {
                    char autor[16], contenido[141];
                    time_t timestamp;
                    sscanf(line, "%s %ld %[^\n]", autor, &timestamp, contenido);
                }
            }
            else if (line[0] == 'T') {
                char seguido[16];
                while (fgets(line, sizeof(line), archivo) && line[0] != ']') {
                    sscanf(line, "%s", seguido);
                    MapPair *pair_seguido = map_search(usuarios, seguido);
                    if (pair_seguido != NULL) {
                        Usuario *usuario_seguido = pair_seguido->value;
                        MapPair *pair_usuario = map_search(usuarios, username);
                        Usuario *usuario = pair_usuario->value;
                        list_pushBack(usuario->seguidos, pair_seguido->value);
                        list_pushBack(usuario_seguido->seguidores, usuario);
                    }
                }
            }
            else if (line[0] == 'S') {
                char seguidor[16];
                while (fgets(line, sizeof(line), archivo) && line[0] != ']') {
                    sscanf(line, "%s", seguidor);
                    }
                }
            
            else if (line[0] == 'N') {
                char notificacion[200];
                while (fgets(line, sizeof(line), archivo) && line[0] != ']') {
                    sscanf(line, "%[^\n]", notificacion);
                }
            }
        }
    }
}
   


void salir(Map *usuarios, FILE *archivo) {
    MapPair *pair = map_first(usuarios);


    while(pair != NULL) {
        Usuario *aux = pair->value;
        fprintf(archivo, "%s %s ", aux->user, aux->pass);
        if (list_first(aux->publicaciones) != NULL) {
            fprintf(archivo, "Publicaciones\n");
        }
        fprintf(archivo, "[");
        Publicacion *aux_pub = list_first(aux->publicaciones);
        while(aux_pub != NULL) {
            char *time = ctime(&aux_pub->timestamp);
            fprintf(archivo, "%s %s %s\n, ", aux_pub->autor, time, aux_pub->contenido);
            aux_pub = list_next(aux->publicaciones);
        }
        fprintf(archivo, "]\n");
        if (list_first(aux->seguidos) != NULL) {
            fprintf(archivo, "Tus Seguidos\n");
        }
        fprintf(archivo, "[");
        Usuario *aux_seguidos = list_first(aux->seguidos);
        while(aux_seguidos != NULL) {
            fprintf(archivo, "%s \n", aux_seguidos->user);
            aux_seguidos = list_next(aux->seguidos);
        }
        fprintf(archivo, "]\n");
        if (list_first(aux->seguidores) != NULL) {
            fprintf(archivo, "Seguidores\n");
        }
        fprintf(archivo, "[");
        Usuario *aux_seguidores = list_first(aux->seguidores);
        while(aux_seguidores != NULL) {
            fprintf(archivo, "%s \n", aux_seguidores->user);
            aux_seguidores = list_next(aux->seguidores);
        }
        fprintf(archivo, "]\n");
        if (queue_front(aux->notificaciones) != NULL) {
            fprintf(archivo, "Notificaciones\n");
        }
        fprintf(archivo, "[");
        char *aux_notificaciones = queue_front(aux->notificaciones);
        while(aux_notificaciones != NULL) {
            fprintf(archivo, "%s \n", aux_notificaciones);
            aux_notificaciones = queue_next(aux->notificaciones);
        }
        fprintf(archivo, "]");
        fprintf(archivo, "\n");
        pair = map_next(usuarios);
    }
    fclose(archivo);
    exit(0);
}


void mostrarMenuInicial(){
  limpiarPantalla();
  puts("========================================");
  puts("     Conecta");
  puts("========================================");

  puts("1) Iniciar Sesión");
  puts("2) Registrar Usuario");
  puts("3) Salir");
}

void mostrarMenuPrincipal(){
  limpiarPantalla();
  puts("========================================");
  puts("     Conecta");
  puts("========================================");

  puts("1) Ver feed");
  puts("2) Publicar mensaje");
  puts("3) Buscar usuario");
  puts("4) Ver notificaciones");
  puts("5) Ver mi perfil");
  puts("6) Sugerencias para ti");
  puts("7) Cerrar sesión");
  puts("8) Salir");
}

int main(){

    FILE *archivo_usuarios = fopen("Usuarios.txt", "r+");

    Map *usuarios = map_create(is_equal_str);
    if (archivo_usuarios != NULL) {
        leerArchivo(usuarios, archivo_usuarios);
    } else {
        printf("No se pudo abrir el archivo de usuarios. Se iniciará con una base de datos vacía.\n");
    }

    
    char opcion; 
    char opcion_inicial;
    int sesion_iniciada = 0; // Variable para controlar si se ha iniciado sesión
    Usuario *usuario_actual = NULL; // Puntero para almacenar el usuario que ha iniciado sesión
    do{
        mostrarMenuInicial();
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion_inicial);
    
        switch (opcion_inicial) {
        case '1':
        sesion_iniciada = iniciarSesion(usuarios, &usuario_actual);
        break;
        case '2':
        sesion_iniciada = registrarUsuario(usuarios, &usuario_actual);
        break;
        case '3':
        fclose(archivo_usuarios);
        archivo_usuarios = fopen("Usuarios.txt", "w");
        salir(usuarios, archivo_usuarios);
        break;
        default:
        printf("Opción inválida.\n");
        break;
        }
        presioneTeclaParaContinuar();
    } while (opcion_inicial != '3' && !sesion_iniciada);


    do {
        mostrarMenuPrincipal();
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);

        switch (opcion) {
        case '1':
            break;
        case '2':
            break;
        case '3':
            break;
        case '4':
            break;
        case '5':
            break;
        case '6':
            break;
        case '7':
            sesion_iniciada = 0; // Marcar que la sesión no está iniciada
            break;
        }
        presioneTeclaParaContinuar();

  } while (opcion != '8' && opcion != '7' && sesion_iniciada);

  fclose(archivo_usuarios);
  archivo_usuarios = fopen("Usuarios.txt", "w");
  salir(usuarios, archivo_usuarios);
  return 0;
}