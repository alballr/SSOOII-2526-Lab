#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "definitions.h"

static const size_t BUFFER_SIZE = 4096;

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
 ************************************************************/

void instalarManejador();
void manejador();

struct FichaEstudiante *gp_tabla_estudiantes = NULL; /*tabla que almacena los datos de todos los estudiantes*/

int main(int argc, char *argv[]){
    int num_estudiantes = 0;
    char path[MAX_PATH];

    instalarManejador();

    if (read(0, &num_estudiantes, sizeof(int)) <= 0) {
        perror("[PA] Error leyendo numero de estudiantes. \n");
        exit(EXIT_FAILURE);
    }
    if((gp_tabla_estudiantes = malloc(num_estudiantes * sizeof(struct FichaEstudiante))) == NULL){
        perror("[PA] Error alocando espacio para las fichas de Estudiantes. \n");
        exit(EXIT_FAILURE);
    }

    if( read(0,gp_tabla_estudiantes,num_estudiantes * sizeof(struct FichaEstudiante)) <= 0 ){
        perror("[PA] Error leyendo lista de estudiantes.  \n");
        free(gp_tabla_estudiantes);
        exit(EXIT_FAILURE);
    }

    for (int estudiante = 0; estudiante < num_estudiantes; estudiante++) {
        snprintf(path, MAX_PATH, "%s/%s", STU_DIR_PATH, gp_tabla_estudiantes[estudiante].dni); 
        mkdir(path, 0775);
    } 
    printf("[PA] Proceso terminado. \n");
    return EXIT_SUCCESS;
}

void instalarManejador(){
    if(signal(SIGINT, manejador) == SIG_ERR){
        perror("[PA] No se pudo establecer el manejador de señales para SIGINT. \n");
        exit(EXIT_FAILURE);
    }  
}

void manejador(int signal){
    if (signal == SIGINT){
        printf("[PA] Terminando el proceso (SIGINT). \n");
        free(gp_tabla_estudiantes);
        exit(EXIT_SUCCESS);
    }
}
