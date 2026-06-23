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
void mostrarMenuPrincipal(Usuario *usuario_actual);
void cerrarSesion(Usuario **usuario_actual);

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
    puts("Sesión iniciada exitosamente.");
    return 1;
}

bool registrarUsuario(Map *usuarios, Usuario **usuario_actual) {
    limpiarPantalla();
    puts("=======================================");
    puts("           Registrar Usuario");
    puts("=======================================");
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
    map_insert(usuarios, strdup(username), nuevo_usuario);
    printf("DEBUG: insertado '%s' en el mapa\n", username);
}

void leerArchivo(Map *usuarios, FILE *archivo) {
    char line[200];

    //primera pasada:
    //se crean los usuarios y se agregan sus publicaciones y notificaciones
    printf("DEBUG: leyendo archivo de usuarios\n");
    while (fgets(line, sizeof(line), archivo)) {
        printf("DEBUG: linea = %s\n", line); 
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
    fclose(archivo);
    archivo = fopen("Usuarios.txt", "w");

    MapPair *pair = map_first(usuarios);


    while(pair != NULL) {
        Usuario *aux = pair->value;
        fprintf(archivo, "USUARIO %s %s\n", aux->user, aux->pass); //imprimir user y pass

        //formato de salida general: "nombre + cantidad", luego en cada siguiente linea se imprime el contenido de cada elemento

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

void cerrarSesion(Usuario **usuario_actual) {
    *usuario_actual = NULL;
    printf("Sesión cerrada exitosamente.\n");
}

void mostrarMenuInicial(){
  limpiarPantalla();
  puts("=======================================");
  puts("                Conecta");
  puts("=======================================");

  puts("1) Iniciar Sesión");
  puts("2) Registrar Usuario");
  puts("3) Salir");
}

void mostrarMenuPrincipal(Usuario *usuario_actual){
  limpiarPantalla();
  puts("=======================================");
  puts("                Conecta");
  puts("=======================================");

  printf("Bienvenido, %s\n", usuario_actual->user);
  puts("1) Ver feed");
  puts("2) Publicar mensaje");
  puts("3) Buscar usuario");
  puts("4) Ver notificaciones");
  puts("5) Ver mi perfil");
  puts("6) Sugerencias para ti");
  puts("7) Cerrar sesión");
  puts("8) Salir");
}

void menuInicial(int *sesion_iniciada, Usuario **usuario_actual, Map *usuarios, FILE *archivo_usuarios) {
    char opcion_inicial = '\0'; //se inicializa para evitar q contenga valores basura
    do{
        mostrarMenuInicial();
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion_inicial);
    
        switch (opcion_inicial) {
        case '1':
            *sesion_iniciada = iniciarSesion(usuarios, usuario_actual);
            break;
        case '2':
            *sesion_iniciada = registrarUsuario(usuarios, usuario_actual);
            break;
        case '3':
            salir(usuarios, archivo_usuarios);
            break;
        default:
            printf("Opción inválida.\n");
            break;
        }
        presioneTeclaParaContinuar();
    } while (opcion_inicial != '3' && !(*sesion_iniciada));
}

int main(){

    FILE *archivo_usuarios = fopen("Usuarios.txt", "r+");

    Map *usuarios = map_create(is_equal_str);
    if (archivo_usuarios != NULL) {
        leerArchivo(usuarios, archivo_usuarios);
    } else {
        printf("No se pudo abrir el archivo de usuarios. Se iniciará con una base de datos vacía.\n");
    }

    
    char opcion = '\0'; //se inicializa para evitar q contenga valores basura
    int sesion_iniciada = 0; // Variable para controlar si se ha iniciado sesión
    Usuario *usuario_actual = NULL; // Puntero para almacenar el usuario que ha iniciado sesión

    int enEjecucion = 1; // Variable para controlar el bucle principal

    while (enEjecucion) {
        menuInicial(&sesion_iniciada, &usuario_actual, usuarios, archivo_usuarios);

        if (!sesion_iniciada) {
            enEjecucion = 0; //si el usuario escoge opcion 3 (salir) en el menu inicial, se sale del programa
            continue; 
        }

        do {
            mostrarMenuPrincipal(usuario_actual);
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
                cerrarSesion(&usuario_actual); // Cerrar sesión
                break;
            case '8':
                enEjecucion = 0; // Marcar que se desea salir del programa
                break;
            }
            presioneTeclaParaContinuar();

        } while (opcion != '8' && sesion_iniciada);

    }
    salir(usuarios, archivo_usuarios);
    return 0;
}