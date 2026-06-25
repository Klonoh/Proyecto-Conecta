#include "extra.h"
#include <time.h>
// Función para limpiar la pantalla
void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void presioneTeclaParaContinuar() {
    puts("Presione una tecla para continuar...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // limpia buffer completo
    getchar(); // espera a que el usuario presione una tecla
}

void formatearFecha(time_t timestamp, char* buffer, int tam) {
    struct tm *info = localtime(&timestamp);
    strftime(buffer, tam, "%d-%m-%Y %H:%M", info);
}


