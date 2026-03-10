PRACTICA 2 - SISTEMAS OPERATIVOS II

1) DESCRIPCION
Programa que busca una palabra en un fichero de texto usando varios hilos.
Divide el archivo por lineas y cada hilo analiza un rango. Por cada ocurrencia,
muestra la linea y las palabras anterior y siguiente (si no existen se imprime
_ como marcador).

2) ESTRUCTURA DEL PROYECTO
- src/
  Código fuente en C++:
  - ssooiigle.cpp : proceso principal, maneja argumentos y crea hilos.
  - searcher.cpp : logica de busqueda por hilo.
  - searchResult.cpp : estructura de resultados.

- include/
  Cabeceras:
  - definitions.hpp : variables compartidas (ruta del archivo).
  - searcher.hpp : interfaz de Searcher.
  - searchResult.hpp : interfaz de SearchResult.

- libros/
  Ficheros de texto de prueba.

- obj/
  Objetos generados durante la compilación.

- exec/
  Archivos ejecutables.

- Makefile
  Automatiza compilación, limpieza y ejecución.

3) COMPILACION
Desde el directorio raiz del proyecto:

  make

4) EJECUCION
Por terminal:

  ./exec/ssooiigle <archivo> <palabra> <num_hilos>

Con Make:

  make execute ARGS="<archivo> <palabra> <num_hilos>"

Ejemplo:

  ./exec/ssooiigle libros/17_LEYES_DEL_TRABAJO_EN_EQUIPO.txt ley 4



5) LIMPIEZA

  make clean

6) REPOSITORIO
Codigo y cambios en:
https://github.com/alballr/SSOOII-2526-Lab
