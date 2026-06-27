# Red social "Conecta"
 
## Descripción
 
Conecta es una aplicación de consola desarrollada en lenguaje C que simula una red social al estilo de X / Twitter. El proyecto nace como respuesta a la necesidad de aplicar estructuras de datos eficientes para resolver operaciones típicas de una red social: gestionar relaciones de seguimiento entre usuarios, mantener un feed ordenado cronológicamente, notificar eventos en tiempo real, recomendar nuevas conexiones relevantes y detectar tendencias dentro del contenido publicado.
 
Cada una de estas funcionalidades impone requisitos distintos de acceso, inserción y eliminación de datos, por lo que el desarrollo de Conecta se centró en seleccionar y justificar, para cada problema, el Tipo de Dato Abstracto (TDA) más adecuado — priorizando la eficiencia y la coherencia con el patrón de uso real de cada operación, antes que la simplicidad de tener una única estructura genérica para todo.
 
El resultado es una aplicación funcional que permite registrar cuentas, publicar contenido, seguir a otros usuarios, recibir notificaciones, navegar un feed personalizado, recibir sugerencias de nuevas conexiones y explorar tendencias mediante hashtags — todo con persistencia de datos entre ejecuciones.
 
---
 
## Requisitos
 
- Compilador de C (se recomienda **gcc**)
- Sistema operativo Linux, Mac o Windows (con MinGW)
---
 
## Compilación
 
```bash
gcc Conecta_main.c Conecta/*.c TDAs/*.c -I Conecta -I TDAs -Wno-unused-result -o Conecta
```
 
## Ejecución
 
```bash
./Conecta
```
 
---
 
## Estructura del proyecto
 

- Conecta_main.c      Punto de entrada del programa (función main).
- Conecta/            Módulos de la aplicación, separados por responsabilidad.
    - entidades.h       Definición de las estructuras Usuario y Publicación.
    - usuarios.c/h      Registro, inicio de sesión y gestión de cuentas.
    - publicaciones.c/h Creación y manejo de publicaciones.
    - sugerencias.c/h   Algoritmo de sugerencias de usuarios (BFS sobre el grafo).
    - tendencias.c/h    Conteo y ranking de hashtags.
    - menu.c/h          Menús de la aplicación y su navegación.
    - guardado.c/h      Persistencia de datos (lectura y escritura del archivo).
    - identificacion.c/h Lógica de autenticación y validación de credenciales.
    - generales.c/h     Funciones de utilidad compartidas entre módulos.
- TDAs/               Implementación de las estructuras de datos genéricas.
    - map.c/h           Mapa (usado para usuarios y grafo de conexiones).
    - list.c/h          Lista enlazada.
    - queue.h           Cola FIFO (implementada sobre la lista).
    - graph.c/h         Grafo dirigido con lista de adyacencia.
    - extra.c/h         Funciones auxiliares (formato de fecha, limpiar pantalla, etc).
- Usuarios.txt        Almacenamiento permanente de los usuarios y sus datos.
- README.md           Este documento.

 
---
 
## Funcionalidades
 
- Registro de usuarios
- Inicio de sesión
- Cerrar sesión
- Publicación de mensajes (con soporte de hashtags)
- Búsqueda de usuarios
- Seguir / dejar de seguir usuarios
- Visualización de perfiles (propio y de terceros)
- Feed de publicaciones, ordenado cronológicamente
- Sistema de notificaciones (orden FIFO)
- Sugerencias de usuarios, priorizadas por cantidad de seguidores en común
- Tendencias mediante hashtags, con ranking de los más usados
- Edición de perfil (cambio de nombre de usuario, contraseña, eliminación de cuenta)
- Persistencia automática de datos en archivo de texto
---
 
## Notas
 
* El programa guarda automáticamente toda la información en `Usuarios.txt` al salir o al cerrar sesión, de modo que los datos persisten entre distintas ejecuciones del programa.
 
---
 
## Ejemplos de uso
 
### Menú Inicial
 
Al ejecutar el programa se muestra el menú inicial, donde se puede registrar un nuevo usuario, iniciar sesión con una cuenta ya existente o salir del programa.
 
```
=======================================
 _____                       _        
/  __ \                     | |       
| /  \/ ___  _ __   ___  ___| |_ __ _ 
| |    / _ \| '_ \ / _ \/ __| __/ _` |
| \__/\ (_) | | | |  __/ (__| || (_| |
\_____/\___/|_| |_|\___|\___|\__\__,_|
 
=======================================
1) Iniciar Sesión
2) Registrar Usuario
3) Salir
Ingrese su opción: 
```
 
### Registro de usuario
 
Se permite crear una nueva cuenta donde el sistema solicitará nombre de usuario, contraseña y confirmación de contraseña. El nombre de usuario debe tener entre 4 y 15 caracteres y solo puede contener letras, números, `_` y `.`.
 
```
=======================================
           Registrar Usuario
=======================================
Ingrese un nombre de usuario (4 a 15 caracteres, letras sin tilde, números, "." y "_"): usuario
Ingrese una contraseña (máximo 20 caracteres): Usuario123
Confirme la contraseña: Usuario123
```
 
### Iniciar sesión
 
Permite ingresar con un usuario ya existente.
 
```
========================================
           Iniciar Sesión
========================================
Ingrese su nombre de usuario: Usuario
Ingrese su contraseña: Usuario123
```
 
Si el usuario existe y la contraseña coincide, se inicia sesión. En caso contrario, se solicita reintentar.
 
### Menú principal
 
Una vez iniciada la sesión, se muestra el menú principal con todas las funciones disponibles.
 
```
=======================================
 _____                       _        
/  __ \                     | |       
| /  \/ ___  _ __   ___  ___| |_ __ _ 
| |    / _ \| '_ \ / _ \/ __| __/ _` |
| \__/\ (_) | | | |  __/ (__| || (_| |
\_____/\___/|_| |_|\___|\___|\__\__,_|
 
=======================================
Bienvenido, usuario.
1) Ver feed
2) Publicar mensaje
3) Buscar usuario
4) Ver notificaciones
5) Ver mi perfil
6) Sugerencias para ti
7) Ver tendencias
8) Cerrar sesión
9) Salir
```
 
### Ver feed
 
Muestra todas las publicaciones realizadas por los usuarios seguidos, ordenadas cronológicamente desde la más reciente hasta la más antigua.
 
```
=======================================
              Mi Feed
=======================================
 
hola:
 
Hoy aprendi que es un grafo!
 
19:55 · 26 Jun, 2026
 
Presione una tecla para continuar...
```
 
### Publicar mensaje
 
Permite a los usuarios publicar un mensaje de hasta 140 caracteres. Cada publicación almacena autor, fecha y contenido. Si el contenido incluye hashtags (palabras precedidas de `#`), estos se registran automáticamente para el sistema de tendencias.
 
```
=======================================
           Publicar Mensaje
=======================================
Ingrese el contenido de su mensaje (máximo 140 caracteres): 
```
 
### Buscar usuario
 
Mediante el nombre de usuario, se puede buscar con coincidencia parcial todos los nombres similares al que se busca.
 
```
=======================================
           Buscar Usuario
=======================================
Ingrese el nombre de usuario a buscar: usua
 
Resultados de la búsqueda:
1) usuario123
2) usuarioejemplo
 
Ingrese el número del perfil que desea ver (0 para cancelar): 
```
 
### Ver perfil de un usuario
 
Una vez encontrado el usuario, se puede ver su perfil con la opción de seguirlo.
 
```
=======================================
               seba
=======================================
Publicaciones: 13 | Seguidores: 37 | Seguidos: 41
=======================================
 
No le sigues
=======================================
 
seba: 
 
por fin salio el sol, ¡estaba harto de la lluvia!
 
07:23 · 27 Jun, 2026
 
=======================================
```
 
### Seguir a un usuario
 
Al seguir a otro usuario, sus publicaciones comenzarán a aparecer en el feed propio.
 
```
1) Seguir
2) Volver al menú principal
 
Ingrese su opción: 1
Ahora sigues a seba.
Presione una tecla para continuar...
```
 
### Dejar de seguir a un usuario
 
Al dejar de seguir, sus publicaciones dejan de aparecer en el feed.
 
```
1) Dejar de seguir
2) Volver al menú principal
 
Ingrese su opción: 1
Has dejado de seguir a seba.
Presione una tecla para continuar...
```
 
### Ver notificaciones
 
Muestra todas las notificaciones pendientes en orden cronológico (orden de llegada).
 
```
=======================================
           Notificaciones
=======================================
El usuario nati_cine te menciono en una publicacion.
El usuario ninatv te menciono en una publicacion.
El usuario nataliacl ha comenzado a seguirte.
El usuario techinsider ha publicado un nuevo mensaje.
Hay nuevos temas populares en tu feed.
FIN
```
 
### Ver mi perfil
 
Permite visualizar publicaciones (mostrando 3 a la vez, con opción de ver más), seguidores y seguidos propios, además de acceder a la edición de perfil.
 
```
=======================================
               usuario
=======================================
Publicaciones: 2 | Seguidores: 2 | Seguidos: 0
=======================================
 
usuario: 
 
¡hola conecta!
 
19:55 · 26 Jun, 2026
 
=======================================
 
1) Ver seguidores
2) Ver seguidos
3) Editar perfil
4) Volver
 
Ingrese su opción: 
```
 
### Ver seguidores / seguidos
 
Permite ver la lista de personas que siguen la cuenta o que la cuenta sigue, con la posibilidad de entrar a sus perfiles.
 
```
=======================================
              Seguidores
=======================================
 1) pepito
 2) Seba
=======================================
Ingrese el número del usuario que desea ver (0 para volver al menú principal): 
```
 
### Editar perfil
 
Se muestran las opciones disponibles para configurar el perfil propio.
 
```
=======================================
            Editar Perfil
=======================================
1) Cambiar nombre de usuario
2) Cambiar contraseña
3) Eliminar cuenta
4) Volver al menú principal
 
Ingrese su opción: 
```
 
### Cambiar nombre de usuario
 
```
Ingrese el nuevo nombre de usuario: usuario1
Nombre de usuario actualizado correctamente.
Presione una tecla para continuar...
```
 
### Cambiar contraseña
 
```
Ingrese su contraseña actual: Usuario123
Ingrese la nueva contraseña: Usuario321
Confirme la nueva contraseña: Usuario321
Contraseña actualizada exitosamente.
```
 
### Eliminar cuenta
 
Elimina la cuenta de forma permanente junto con toda su información asociada (publicaciones, relaciones de seguimiento y notificaciones).
 
```
Esta acción eliminará permanentemente su cuenta.
Escriba "confirmo" para continuar: confirmo
Presione una tecla para continuar...
```
 
### Sugerencias para ti
 
El sistema recorre el grafo de conexiones a partir del usuario actual mediante BFS, e identifica usuarios que aún no son seguidos pero que comparten seguidores en común con el usuario actual. Las sugerencias se ordenan priorizando a quienes tienen mayor cantidad de conexiones compartidas.
 
```
=======================================
        Sugerencias para ti
=======================================
 1) Benja1234 - 2 seguidores en común
 2) joacoo - 1 seguidores en común
 3) sebita123 - 1 seguidores en común
=======================================
 
Ingrese el número del usuario que desea ver (0 para volver): 
```
 
### Ver tendencias
 
El sistema analiza todas las publicaciones registradas, extrae automáticamente los hashtags contenidos en ellas y calcula cuántas veces aparece cada uno. Se muestra un ranking con los 10 hashtags más utilizados.
 
```
=======================================
              Tendencias
=======================================
 1) #timeline - 159 publicaciones
 2) #cine - 153 publicaciones
 3) #memes - 145 publicaciones
 4) #robotica - 141 publicaciones
 5) #tendencias - 140 publicaciones
=======================================
 
Ingrese el número del hashtag que desea ver (0 para volver):
```
 
### Ver publicaciones de una tendencia
 
Al seleccionar un hashtag, se muestran las publicaciones que lo contienen, en bloques de 5. El usuario puede elegir ver más publicaciones del mismo hashtag hasta agotar todas las disponibles.
 
```
=======================================
        Publicaciones con #chile
=======================================
 
sofinet:
 
La carpeta final version definitiva ahora si final nunca es final version extendida #Chile
 
05:04 · 27 Jun, 2026
=======================================
 
Mostrando 5 de 135 publicaciones.
1) Ver más
2) Salir
Ingrese su opción: 
```
 
### Cerrar sesión
 
Guarda todos los datos del usuario (publicaciones, seguidores, seguidos, notificaciones) en `Usuarios.txt` y devuelve al menú inicial sin terminar el programa.
 
```
Ingrese su opción: 8
Sesión cerrada exitosamente.
Presione una tecla para continuar...
```
 
### Salir
 
Guarda todos los datos de todos los usuarios y termina la ejecución del programa.
 
```
Ingrese su opción: 9
Presione una tecla para continuar...
```
 
---
 
## Estructuras de datos utilizadas
 
### Mapa
 
Se utiliza un mapa para almacenar y acceder rápidamente a todos los usuarios registrados en la aplicación. Se usa el nombre de usuario como clave y la entidad Usuario correspondiente como valor. El uso de este TDA permite que operaciones como iniciar sesión, registrar usuarios y buscar usuarios por nombre tengan una complejidad temporal de O(1) en promedio para el acceso directo por clave.
 
### Lista
 
Este TDA se utiliza para la lista de seguidos y de seguidores de cada usuario. Esta elección permite insertar y eliminar relaciones de forma directa sobre los nodos, así como recorrer la lista cuando se requiera mostrar las conexiones. En el caso de la lista de publicaciones, se usa de la misma forma con inserción al final (mediante puntero a tail), lo que permite que las publicaciones se agreguen con complejidad temporal O(1), quedando en orden de creación de forma natural.
 
### Cola
 
Se usa una cola FIFO (implementada sobre la lista) para gestionar las notificaciones de cada usuario. Este TDA resulta adecuado porque garantiza que las notificaciones se procesen en el mismo orden en que fueron generadas (primero en llegar, primero en mostrarse). Las operaciones de encolar (nueva notificación) y desencolar (mostrar y eliminar) son O(1).
 
### Grafo
 
Se usa un grafo dirigido, implementado con lista de adyacencia, para modelar las relaciones de seguimiento entre los usuarios de la red social. Cada vértice representa un usuario y cada arista dirigida representa una relación de seguimiento. Se eligió la lista de adyacencia en lugar de una matriz de adyacencia porque las redes sociales son grafos dispersos: cada usuario sigue solo a una fracción del total de usuarios, por lo que una matriz gastaría memoria de forma innecesaria. Esta estructura habilita la funcionalidad de sugerencias, mediante un recorrido BFS que explora la red en niveles de proximidad y prioriza a los candidatos con más conexiones en común con el usuario actual.
 
---
 
## Limitaciones conocidas
 
- **Persistencia en archivo de texto:** el almacenamiento de usuarios y sus datos se realiza en un archivo `.txt` plano (`Usuarios.txt`), leído y escrito completo en cada ejecución. Esta solución es adecuada para el alcance y volumen de datos de este proyecto, pero no es escalable: a medida que crece el número de usuarios y publicaciones, el tiempo de carga y guardado del archivo completo aumenta de forma lineal, y no permite consultas parciales ni acceso concurrente. Una versión más robusta de la aplicación debería migrar a un sistema de base de datos.
- **Sesión única:** la aplicación no soporta múltiples sesiones simultáneas; solo un usuario puede estar autenticado a la vez por ejecución del programa.
- **Sin multimedia:** las publicaciones solo admiten texto (hasta 140 caracteres), sin soporte para imágenes, video u otro contenido adjunto.
- **Sin mensajería privada:** no existe una funcionalidad de mensajes directos entre usuarios.
---
 
## Contribuciones
 
* Sebastián Arriagada:
     - Organización e implementación de TDAs
     - Implementación de función Ver Feed
     - Implementación de función Estadísticas de la Cuenta
     - Implementación de función Leer Archivo
     - Implementación de función Salir
     - Gran aporte en la idea de creación de material presentado sobre las funciones creadas.
     - Soluci
     - Realización del informe

* Joaquín Carrillo:
     - Implementación de función Registrar Usuario
     - Implementación de función Iniciar Sesión
     - Implementación de función Sugerencias de Usuarios
     - Implementación de función Buscar Usuarios
     - Implementación de función Leer Archivo
     - Implementación de función Salir
     - Implementacion de función Mostrar Tendencias.
     - Implementacion de función de cambiar nombre de usuario
     - Implementacion de función de cambiar contraseña
     - Realización del informe 
     - Gran aporte en la creación de material presentados en el orden y muestra de resultados obtenidos.
     - Gran aporte en comentar las funciones del código
     - Coordinación de las tareas a realizar del equipo
     - Resolución de problemas producidos por la inexperiencia general del equipo con la herramienta de git

* Benjamín Beltrán:
     - Implementación de funciones para hacer uso de un menú funcional
     - Implementación de función Gestionar Seguimientos
     - Implementación de función Publicar Mensaje
     - Implementación de función Ver Notificaciones
     - Implementación de función Cerrar Sesión
     - Creador principal del material CANVA presentado en clases siendo los avances y las funciones principales del proyecto.
     - Gran aporte en comentar las funciones del código
     - Realización del informe
     - Creador principal del archivo README.
