PRACTICA 1 - SISTEMAS OPERATIVOS II

1) ESTRUCTURA DEL PROYECTO
- src/
  Código fuente en C:
  - manager.c : proceso principal, Crea y coordina los procesos.
  - pa.c      : Crea un directorio por estudiante.
  - pb.c      : Copia el modelo de examen según grupo del estudiante.
  - pc.c      : Genera notas y calcula nota media.

- include/
  - definitions.h : constantes compartidas y definición del struct FichaEstudiante.

- examenes/
  Modelos de examen (A.pdf, B.pdf, C.pdf).

- estudiantes.txt
  Fichero de entrada con la lista de estudiantes.

- obj/
  Objetos generados durante la compilación.

- exec/
  Archivos ejecutables.

- backup/
  Copias de seguridad creadas por el daemon del manager.

- log.txt
  Registro de ejecución generado por manager.

- Makefile
  Automatiza compilación, limpieza y ejecución.


2) COMPILACION
Desde el directorio raíz del proyecto:

  make

Esto crea las carpetas obj/ y exec/ y compila todos los ejecutables.

También se puede compilar por clase:

  make manager
  make pa
  make pb
  make pc


3) EJECUCIÓN Y LIMPIEZA
Ejecución completa del proyecto:

  make execute


4) LIMPIEZA
Limpiar binarios, objetos y temporales:

  make clean

Limpiar solo contenido de ./estudiantes:

  make clean-students

5) REPOSITORIO
Todo el código junto con los cambios hechos se encuentra en el repositorio: https://github.com/alballr/SSOOII-2526-Lab
