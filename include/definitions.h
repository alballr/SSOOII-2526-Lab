#define STU_FILE_PATH "/home/alballr/SSOO/SSOOII-2526-Lab/estudiantes.txt"
#define STU_DIR_PATH "/home/alballr/SSOO/SSOOII-2526-Lab/estudiantes"

#define LECTURA 0
#define ESCRITURA 1

#define MAX_PATH 4096 /*tamaño máximo del path del archivo*/

struct FichaEstudiante {          
    char dni[32];
    char grupo;
    int nota;   
};