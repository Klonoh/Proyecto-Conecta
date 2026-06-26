#include <stdio.h>

#include "menu.h"
#include "identificacion.h"
#include "guardado.h"
#include "TDAs/extra.h"

void mostrarMenuInicial(){
    limpiarPantalla();
    puts("=======================================");
    printf(" _____                       _        \n"
        "/  __ \\                     | |       \n"
        "| /  \\/ ___  _ __   ___  ___| |_ __ _ \n"
        "| |    / _ \\| '_ \\ / _ \\/ __| __/ _` |\n"
        "| \\__/\\ (_) | | | |  __/ (__| || (_| |\n"
        "\\_____/\\___/|_| |_|\\___|\\___|\\__\\__,_|\n\n");
    puts("=======================================");

    puts("1) Iniciar Sesión");
    puts("2) Registrar Usuario");
    puts("3) Salir");
}

void mostrarMenuPrincipal(Usuario *usuario_actual){
  
    limpiarPantalla();
    puts("=======================================");
    printf(" _____                       _        \n"
        "/  __ \\                     | |       \n"
        "| /  \\/ ___  _ __   ___  ___| |_ __ _ \n"
        "| |    / _ \\| '_ \\ / _ \\/ __| __/ _` |\n"
        "| \\__/\\ (_) | | | |  __/ (__| || (_| |\n"
        "\\_____/\\___/|_| |_|\\___|\\___|\\__\\__,_|\n\n");
    puts("=======================================");

    printf("Bienvenido, %s\n", usuario_actual->user);
    puts("1) Ver feed");
    puts("2) Publicar mensaje");
    puts("3) Buscar usuario");
    puts("4) Ver notificaciones");
    puts("5) Ver mi perfil");
    puts("6) Sugerencias para ti");
    puts("7) Ver tendencias");
    puts("8) Cerrar sesión");
    puts("9) Salir");
}

void menuInicial(int *sesion_iniciada, Usuario **usuario_actual, Map *usuarios, FILE *archivo_usuarios, Graph *grafo) {
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
            if (*sesion_iniciada) {
                addNode(grafo, (*usuario_actual)->user);
            }
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