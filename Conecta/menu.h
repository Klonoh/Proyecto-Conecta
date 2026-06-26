#ifndef MENU_H
#define MENU_H

#include <stdio.h>

#include "entidades.h"
#include "map.h"
#include "graph.h"

void mostrarMenuInicial(void);

void mostrarMenuPrincipal(Usuario *usuario_actual);

void menuInicial(int *sesion_iniciada, Usuario **usuario_actual, Map *usuarios, FILE *archivo_usuarios, Graph *grafo);

#endif