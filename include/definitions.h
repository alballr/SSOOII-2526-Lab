#define STU_FILE_PATH "/home/alballr/SSOO/SSOOII-2526-Lab/estudiantes.txt"
#define STU_DIR_PATH "/home/alballr/SSOO/SSOOII-2526-Lab/estudiantes"

#define NUM_PROCESOS 2 /*necesario??*/

#define LECTURA 0
#define ESCRITURA 1

struct FichaEstudiante {          
    char dni[32];
    char grupo;
    int nota;   
};