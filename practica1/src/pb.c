#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include "definitions.h"

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

void instalarManejador();
void manejador();

struct FichaEstudiante *gp_tabla_estudiantes = NULL; /*tabla que almacena los datos de todos los estudiantes*/

int main(int argc, char* argv[]){
    int num_estudiantes = 0;
    char str_copy[MAX_PATH];

    instalarManejador();
    
    if(read(0, &num_estudiantes, sizeof(int)) <= 0) {
        perror("[PB] Error leyendo numero de estudiantes. \n");
        exit(EXIT_FAILURE);
    }
    if((gp_tabla_estudiantes = malloc(num_estudiantes * sizeof(struct FichaEstudiante))) == NULL){
        perror("[PB] Error alocando espacio para las fichas de Estudiantes. \n");
        exit(EXIT_FAILURE);
    }

    if( read(0,gp_tabla_estudiantes,num_estudiantes * sizeof(struct FichaEstudiante)) <= 0 ){
        perror("[PB] Error leyendo lista de estudiantes. \n");
        free(gp_tabla_estudiantes);
        exit(EXIT_FAILURE);
    }
    
    for (int estudiante = 0; estudiante < num_estudiantes; estudiante++) {
        switch(gp_tabla_estudiantes[estudiante].grupo){
            case 'A':
                snprintf(str_copy, MAX_PATH, "cp examenes/A.pdf %s/%s", STU_DIR_PATH, gp_tabla_estudiantes[estudiante].dni);
                break;
            case 'B':
                snprintf(str_copy, MAX_PATH, "cp examenes/B.pdf %s/%s", STU_DIR_PATH, gp_tabla_estudiantes[estudiante].dni);
                break;
            case 'C':
                snprintf(str_copy, MAX_PATH, "cp examenes/C.pdf %s/%s", STU_DIR_PATH, gp_tabla_estudiantes[estudiante].dni);
                break;
            default:
                fprintf(stderr,"Error. Existe un alumno en el archivo de estudiantes con grupo incorrecto. El programa terminará \n");
                free(gp_tabla_estudiantes);
                return EXIT_FAILURE;
        }
        system(str_copy);    
    }

    if (kill(getppid(), SIGUSR1) == -1) {
        perror("[PB] Error enviando señal al manager.\n");
        free(gp_tabla_estudiantes);
        return EXIT_FAILURE;
    }

    sleep(5); /* Para poder probar el Ctrl + C*/
    printf("[PB] Proceso terminado. \n");
    free(gp_tabla_estudiantes);
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
        free(gp_tabla_estudiantes);
        printf("[PB] Terminando el proceso (SIGINT) \n");
        exit(EXIT_SUCCESS);
    }
}
