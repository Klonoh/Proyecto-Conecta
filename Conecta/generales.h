#ifndef GENERALES_H
#define GENERALES_H

#include <stdbool.h>

int is_equal_str(void *key1, void *key2);

bool usernameValido(const char *username);

void convertirUsernameMinusculas(char *username);

bool hashtagValido(char c);

int ordenar(const void *a, const void *b);

int ordenarSugerencias(const void *a, const void *b);

int ordenarTendencias(const void *a, const void *b);

#endif