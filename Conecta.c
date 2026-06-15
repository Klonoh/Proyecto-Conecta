# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <string.h>
# include "TDAs/list.h"
# include "TDAs/map.h"
# include "TDAs/queue.h"
# include "TDAs/stack.h"
# include "TDAs/extra.h"

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

    char opcion; 
    char opcion_inicial;
    do{
        mostrarMenuInicial();
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion_inicial);
    
        switch (opcion_inicial) {
        case '1':
        break;
        case '2':
        break;
        case '3':
        printf("Saliendo...\n");
        return 0;
        default:
        printf("Opción inválida.\n");
        break;
        }
        presioneTeclaParaContinuar();
    } while (opcion_inicial != '3');


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
            break;
        }
        presioneTeclaParaContinuar();

  } while (opcion != '8');
  return 0;
}