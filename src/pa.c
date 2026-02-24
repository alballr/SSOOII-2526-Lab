#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "definitions.h"

#define BUFFER_SIZE 4096

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

int main(int argc, char *argv[]){
    int num_estudiantes = 0;
    int i = 0; 
    char path[MAX_PATH];
    struct FichaEstudiante *p_tabla_estudiantes = NULL; /*tabla que almacena los datos de todos los estudiantes*/

    instalarManejador();

    if (read(0, &num_estudiantes, sizeof(int)) <= 0) {
        perror("[PA] Error leyendo numero de estudiantes. \n");
        exit(EXIT_FAILURE);
    }
    if((p_tabla_estudiantes = malloc(num_estudiantes * sizeof(struct FichaEstudiante))) == NULL){
        perror("[PA] Error alocando espacio para las fichas de Estudiantes. \n");
        exit(EXIT_FAILURE);
    }

    if( read(0,p_tabla_estudiantes,num_estudiantes * sizeof(struct FichaEstudiante)) <= 0 ){
        perror("[PA] Error leyendo lista de estudiantes.  \n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < num_estudiantes; i++) {
        snprintf(path, MAX_PATH, "%s/%s", STU_DIR_PATH, p_tabla_estudiantes[i].dni); 
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
        exit(EXIT_SUCCESS);
    }
}
