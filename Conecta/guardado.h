#ifndef GUARDADO_H
#define GUARDADO_H

#include <stdio.h>

#include "map.h"

void leerArchivo(Map *usuarios, FILE *archivo);

void salir(Map *usuarios, FILE *archivo);

#endif