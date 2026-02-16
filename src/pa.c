#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <definitions.h>

#define BUFFER_SIZE 4096
#define MAX_PATH 4096

/************************************************************
 * Project        : Practica 1 de Sistemas Operativos II
 *
 * Program name   : pa.c
 *
 * Author         : Alba Llorente
 *
 * Date created   : 09/02/2026
 *
 * Purpose        : Código del proceso A encargado de la creación de un directorio por estudiante
 *
 * Revision History :
 *
 * Date        Author    Ref   Revision
 * 10/02/2026  Alba   1      Paso de la lectura del archivo estudiantes.txt a manager.c
 * 16/02/2026  Alba   2      Creación de los procesos B y C
 *
 ************************************************************/

int main(int argc, char *argv[]){
    int num_estudiantes = 0;
    int i = 0; 
    char path[MAX_PATH];
    struct FichaEstudiante *p_tabla_estudiantes = NULL; /*tabla que almacena los datos de todos los estudiantes*/

    if (read(0, &num_estudiantes, sizeof(int)) <= 0) {
        perror("[PA] Error leyendo numero de estudiantes");
        exit(EXIT_FAILURE);
    }
    if((p_tabla_estudiantes = malloc(num_estudiantes * sizeof(struct FichaEstudiante))) == NULL){
        perror("[PA] Error alocando espacio para las fichas de Estudiantes\n");
        exit(EXIT_FAILURE);
    }

    if( read(0,p_tabla_estudiantes,num_estudiantes * sizeof(struct FichaEstudiante)) <= 0 ){
        perror("[PA] Error leyendo lista de estudiantes");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < num_estudiantes; i++) {
        snprintf(path, MAX_PATH, "%s/%s", STU_DIR_PATH, p_tabla_estudiantes[i].dni); 
        mkdir(path, 0775);
    }
    
    return EXIT_SUCCESS;
}
