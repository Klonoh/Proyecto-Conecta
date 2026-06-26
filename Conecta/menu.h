#ifndef MENU_H
#define MENU_H

#include <stdio.h>

#include "entidades.h"
#include "TDAs/map.h"
#include "TDAs/graph.h"

void mostrarMenuInicial(void);

void mostrarMenuPrincipal(Usuario *usuario_actual);

void menuInicial(int *sesion_iniciada, Usuario **usuario_actual, Map *usuarios, FILE *archivo_usuarios, Graph *grafo);

#endif