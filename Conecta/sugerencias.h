#ifndef SUGERENCIAS_H
#define SUGERENCIAS_H

#include "entidades.h"
#include "TDAs/map.h"
#include "TDAs/graph.h"


void sugerenciasParaTi(Usuario **usuario_actual, Map *usuarios, Graph **grafo, int *sesion_iniciada);

void construirGrafoDesdeUsuarios(Map *usuarios, Graph *grafo);

void reconstruirGrafo(Map *usuarios, Graph **grafo);

#endif