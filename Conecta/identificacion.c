#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "identificacion.h"
#include "usuarios.h"
#include "generales.h"
#include "TDAs/extra.h"

bool iniciarSesion(Map *usuarios, Usuario **usuario_actual) {
    limpiarPantalla();
    puts("========================================");
    puts("           Iniciar Sesión");
    puts("========================================");
    char username[16];
    char password[21];
    printf("Ingrese su nombre de usuario: ");
    scanf("%15s", username);

    convertirUsernameMinusculas(username);

    printf("Ingrese su contraseña: ");
    scanf("%20s", password);
    MapPair *pair = map_search(usuarios, username);
    if (pair == NULL) {
        printf("El nombre de usuario no existe. Intente nuevamente.\n");
        return 0;
    }
    Usuario *usuario = pair->value;
    if (strcmp(usuario->pass, password) != 0) {
        printf("Contraseña incorrecta. Intente nuevamente.\n");
        return 0;
    }
    *usuario_actual = usuario;
    puts("Sesión iniciada exitosamente.");
    return 1;
}

bool registrarUsuario(Map *usuarios, Usuario **usuario_actual) {
    limpiarPantalla();
    puts("=======================================");
    puts("           Registrar Usuario");
    puts("=======================================");

    char username[16];
    char username_input[50];
    char password[21];
    char confirmar_pass[21];

    while (1) {
        printf("Ingrese un nombre de usuario: (4 a 15 caracteres, letras sin tilde, números, \".\" y \"_\"): ");
        scanf("%49s", username_input);

        if (!usernameValido(username_input)) {
            printf("Nombre de usuario inválido.\n");
            continue;
        }

        convertirUsernameMinusculas(username_input);

        if (map_search(usuarios, username_input) != NULL) {
            printf("El nombre de usuario ya existe. Intente con otro.\n");
            continue;
        }

        strcpy(username, username_input);
        break;
    }
    printf("Ingrese una contraseña: (máximo 20 caracteres) ");
    scanf("%20s", password);

    printf("Confirme la contraseña: ");
    scanf("%20s", confirmar_pass);

    if (strcmp(password, confirmar_pass) != 0) {
        printf("Las contraseñas ingresadas no coinciden entre sí. Intente nuevamente.\n");
        return 0;
    }

    inicializarUsuario(usuarios, username, password);
    printf("Usuario registrado exitosamente.\n");
    
    *usuario_actual = map_search(usuarios, username)->value;
    return 1;
}

void cerrarSesion(Usuario **usuario_actual) {
    *usuario_actual = NULL;
    printf("Sesión cerrada exitosamente.\n");
}
