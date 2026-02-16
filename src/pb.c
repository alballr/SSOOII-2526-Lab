#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

/************************************************************
 * Project        : Practica 1 de Sistemas Operativos II
 *
 * Program name   : pb.c
 *
 * Author         : Alba Llorente
 *
 * Date created   : 16/02/2026
 *
 * Purpose        : Código del proceso B encargado de copiar el modelo de examen a cada carpeta.
 ************************************************************/
#define MAX_PATH 4096

void instalarManejador();
void manejador();

 int main(int argc, char* argv[]){
    int num_estudiantes = 0;
    int i; 
    char str_copy[MAX_PATH];
    struct FichaEstudiante *p_tabla_estudiantes = NULL; /*tabla que almacena los datos de todos los estudiantes*/

    instalarManejador();
    
    if(read(0, &num_estudiantes, sizeof(int)) <= 0) {
        perror("[PB] Error leyendo numero de estudiantes\n");
        exit(EXIT_FAILURE);
    }
    if((p_tabla_estudiantes = malloc(num_estudiantes * sizeof(struct FichaEstudiante))) == NULL){
        perror("[PB] Error alocando espacio para las fichas de Estudiantes\n");
        exit(EXIT_FAILURE);
    }

    if( read(0,p_tabla_estudiantes,num_estudiantes * sizeof(struct FichaEstudiante)) <= 0 ){
        perror("[PB] Error leyendo lista de estudiantes\n");
        exit(EXIT_FAILURE);
    }
    
    for (i = 0; i < num_estudiantes; i++) {
        switch(p_tabla_estudiantes[i].grupo){
            case 'A':
                snprintf(str_copy, MAX_PATH, "cp examenes/A.pdf %s/%s", STU_DIR_PATH, p_tabla_estudiantes[i].dni);
                break;
            case 'B':
                snprintf(str_copy, MAX_PATH, "cp examenes/B.pdf %s/%s", STU_DIR_PATH, p_tabla_estudiantes[i].dni);
                break;
            case 'C':
                snprintf(str_copy, MAX_PATH, "cp examenes/C.pdf %s/%s", STU_DIR_PATH, p_tabla_estudiantes[i].dni);
                break;
        }
        system(str_copy);    
    }
    sleep(5);
    printf("[PB] Proceso terminado \n");
    return EXIT_SUCCESS;    
}

void instalarManejador(){
    if(signal(SIGINT, manejador) == SIG_ERR){
        perror("[PB] No se pudo establecer el manejador de señales para SIGINT\n");
        exit(EXIT_FAILURE);
    }  
}

void manejador(int signal){
    if (signal == SIGINT){
        printf("[PB] Terminando el proceso (SIGINT) \n");
        exit(EXIT_SUCCESS);
    }
}
