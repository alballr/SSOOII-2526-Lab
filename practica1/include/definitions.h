#define STU_FILE_PATH "./estudiantes.txt"
#define STU_DIR_PATH "./estudiantes"
#define ARCHIVO_LOG "log.txt"
#define ARCHIVO_NOTAS "notas.txt" /*nombre del archivo de notas*/

#define LECTURA 0
#define ESCRITURA 1

#define MAX_PATH 4096 /*tamaño máximo del path del archivo*/

struct FichaEstudiante {          
    char dni[32];
    char grupo;
    int nota;   
};