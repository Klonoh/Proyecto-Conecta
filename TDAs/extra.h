#ifndef EXTRA_H
#define EXTRA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "list.h"

// Función para limpiar la pantalla
void limpiarPantalla();

void presioneTeclaParaContinuar();

void formatearFecha(time_t timestamp, char* buffer, int tam);

#endif