#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "generales.h"
#include "entidades.h"

int is_equal_str(void *key1, void *key2){
  return strcmp((char *)key1, (char *)key2) == 0;
}

bool usernameValido(const char *username) {
    int largo = strlen(username);

    if (largo < 4 || largo > 15) {
        return false;
    }

    for (int i = 0; username[i] != '\0'; i++) {
        unsigned char c = (unsigned char) username[i];

        bool es_letra = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
        bool es_numero = (c >= '0' && c <= '9');
        bool es_simbolo_valido = (c == '.' || c == '_');

        if (!es_letra && !es_numero && !es_simbolo_valido) {
            return false;
        }
    }

    return true;
}

void convertirUsernameMinusculas(char *username) {
    for (int i = 0; username[i] != '\0'; i++) {
        if (username[i] >= 'A' && username[i] <= 'Z') {
            username[i] = username[i] + 32;
        }
    }
}

bool hashtagValido(char c) {
    bool es_letra = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    bool es_numero = (c >= '0' && c <= '9');
    bool es_guion_bajo = (c == '_');

    return es_letra || es_numero || es_guion_bajo;
}


