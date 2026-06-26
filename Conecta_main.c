#include <stdio.h>

#ifdef _WIN32 //si se está compilando en Windows, se incluye la libreria windows.h
    #include <windows.h>
#endif

#include "Conecta/entidades.h"
#include "Conecta/generales.h"
#include "Conecta/identificacion.h"
#include "Conecta/usuarios.h"
#include "Conecta/publicaciones.h"
#include "Conecta/sugerencias.h"
#include "Conecta/tendencias.h"
#include "Conecta/guardado.h"
#include "Conecta/menu.h"

#include "TDAs/map.h"
#include "TDAs/graph.h"
#include "TDAs/extra.h"

int main() {
    #ifdef _WIN32 //si es windows, se establece la codificación de salida a UTF-8 para que se muestren correctamente los caracteres especiales
        SetConsoleOutputCP(CP_UTF8);
    #endif

    FILE *archivo_usuarios = fopen("Usuarios.txt", "r+");

    Map *usuarios = map_create(is_equal_str);

    if (archivo_usuarios != NULL) {
        leerArchivo(usuarios, archivo_usuarios);
    } else {
        printf("No se pudo abrir el archivo de usuarios. Se iniciará con una base de datos vacía.\n");
    }

    Graph *grafo = createGraph();
    construirGrafoDesdeUsuarios(usuarios, grafo);

    int opcion = 0; //se inicializa para evitar q contenga valores basura
    int sesion_iniciada = 0; // Variable para controlar si se ha iniciado sesión
    Usuario *usuario_actual = NULL; // Puntero para almacenar el usuario que ha iniciado sesión
    int enEjecucion = 1; // Variable para controlar el bucle principal

    while (enEjecucion) {
        menuInicial(&sesion_iniciada, &usuario_actual, usuarios, archivo_usuarios, grafo);

        if (usuario_actual != NULL) {
            addNode(grafo, usuario_actual->user);
        }

        if (!sesion_iniciada) {
            enEjecucion = 0; //si el usuario escoge opcion 3 (salir) en el menu inicial, se sale del programa
            continue;
        }

        do {
            mostrarMenuPrincipal(usuario_actual);
            printf("Ingrese su opción: ");
            if (scanf(" %d", &opcion) != 1) {
                opcion = 0; // Opción inválida, se reinicia a 0
            }
            
            int c;
            while ((c = getchar()) != '\n' && c != EOF); // Limpiar el buffer de entrada

            switch (opcion) {
                case 1:
                    verFeed(usuario_actual);
                    break;
                case 2:
                    publicarMensaje(usuario_actual);
                    break;
                case 3:
                    buscarUsuario(usuarios, &usuario_actual, &sesion_iniciada, &grafo);
                    break;
                case 4:
                    verNotificaciones(usuario_actual);
                    break;
                case 5:
                    MostrarPerfil(&usuario_actual, usuario_actual, usuarios, &sesion_iniciada, &grafo);
                    break;
                case 6:
                    sugerenciasParaTi(&usuario_actual, usuarios, &grafo, &sesion_iniciada);
                    break;
                case 7:
                    verTendencias(usuarios);
                    break;
                case 8:
                    sesion_iniciada = 0;
                    cerrarSesion(&usuario_actual);
                    break;
                case 9:
                    enEjecucion = 0;
                    break;
                default:
                    printf("Opción inválida.\n");
                    break;
            }

            presioneTeclaParaContinuar();

        } while (opcion != 9 && sesion_iniciada);
    }

    salir(usuarios, archivo_usuarios);
    return 0;
}