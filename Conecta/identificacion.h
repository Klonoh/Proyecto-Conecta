#ifndef IDENTIFICACION_H
#define IDENTIFICACION_H

#include <stdbool.h>
#include "entidades.h"
#include "map.h"

bool iniciarSesion(Map *usuarios, Usuario **usuario_actual);

bool registrarUsuario(Map *usuarios, Usuario **usuario_actual);

void cerrarSesion(Usuario **usuario_actual);

#endif