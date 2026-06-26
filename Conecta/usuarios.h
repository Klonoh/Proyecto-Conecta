#ifndef USUARIOS_H
#define USUARIOS_H

#include <stdbool.h>

#include "entidades.h"
#include "map.h"
#include "list.h"
#include "graph.h"

void inicializarUsuario(Map *usuarios, char *username, char *password);

void buscarUsuario(Map *usuarios, Usuario **usuario_actual, int *sesion_iniciada, Graph **grafo);

void MostrarPerfil(Usuario **usuario_actual, Usuario *usuario, Map *usuarios, int *sesion_iniciada, Graph **grafo);

void seguirUsuario(Usuario *usuario_actual, Usuario *usuario_a_seguir, Graph *grafo);

void dejarDeSeguirUsuario(Usuario *usuario_actual, Usuario *usuario_a_dejar_de_seguir, Graph *grafo);

void verListaUsuarios(Usuario **usuario_actual, List *lista, const char *titulo, const char *mensaje_vacio, Map *usuarios, int *sesion_iniciada, Graph **grafo);

void editarPerfil(Usuario **usuario_actual, Map *usuarios, int *sesion_iniciada, Graph **grafo);

bool yaSigue(Usuario *usuario_actual, const char *username);

#endif