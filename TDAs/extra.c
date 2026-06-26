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
    fseek(stdin, 0, SEEK_END);
    getchar(); // espera a que el usuario presione una tecla
}

void formatearFecha(time_t timestamp, char* buffer, int tam) {
    struct tm *info = localtime(&timestamp);
    strftime(buffer, tam, "%H:%M · %d %b, %Y", info);
}


