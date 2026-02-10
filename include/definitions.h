#define STU_FILE_PATH "./estudiantes.txt"
#define STU_DIR_PATH "./estudiantes"
#define NUM_PROCESOS 2 //necesario??

struct FichaEstudiante {          
    char dni[32];
    char grupo;
    int nota;   
};