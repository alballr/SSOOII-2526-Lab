#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "definitions.h"

/************************************************************
 * Project        : Practica 1 de Sistemas Operativos II
 *
 * Program name   : pc.c
 *
 * Author         : Alba Llorente
 *
 * Date created   : 16/02/2026
 *
 * Purpose        : Código del proceso C encargado del cálculo de nota minima para aprobar por estudiante
 ************************************************************/

static const size_t MAX_STR = 512; /*tamaño maximo del str de notas*/
static const char NOTAS_STR[] = "La nota que debes obtener en este nuevo examen para superar la prueba es"; /*texto que acompaña el archivo de notas*/

void instalarManejador();
void manejador();

struct FichaEstudiante *gp_tabla_estudiantes = NULL; /*tabla que almacena los datos de todos los estudiantes*/
    
int main(int argc, char *argv[]){
    int num_estudiantes = 0;
    int nota;
    double media_total = 0;
    char path_notas[MAX_PATH];
    char str_notas[MAX_STR];
    FILE *notas_file; 

    instalarManejador();
    
    if(read(0, &num_estudiantes, sizeof(int)) <= 0) {
        perror("[PC] Error leyendo numero de estudiantes. \n");
        exit(EXIT_FAILURE);
    }
    if((gp_tabla_estudiantes = malloc(num_estudiantes * sizeof(struct FichaEstudiante))) == NULL){
        perror("[PC] Error alocando espacio para las fichas de Estudiantes. \n");
        exit(EXIT_FAILURE);
    }

    if( read(0,gp_tabla_estudiantes,num_estudiantes * sizeof(struct FichaEstudiante)) <= 0 ){
        perror("[PC] Error leyendo lista de estudiantes. \n");
        free(gp_tabla_estudiantes);
        exit(EXIT_FAILURE);
    }

    for (int estudiante = 0; estudiante < num_estudiantes; estudiante++) {
        snprintf(path_notas, MAX_PATH, "%s/%s/%s", STU_DIR_PATH, gp_tabla_estudiantes[estudiante].dni, ARCHIVO_NOTAS);
        nota = 2 * 5 - gp_tabla_estudiantes[estudiante].nota;
        media_total += gp_tabla_estudiantes[estudiante].nota; /*aquí solo sumamos las notas de todos los estudiantes*/

        if((notas_file = fopen(path_notas, "w")) == NULL){
            free(gp_tabla_estudiantes);
            perror("[PC] Error creando el archivo de notas\n");
            exit(EXIT_FAILURE);
        }
        fprintf(notas_file, "%s %d\n", NOTAS_STR, nota);
        fclose(notas_file);
    }

    media_total = media_total / num_estudiantes;
    snprintf(str_notas, MAX_STR,"%f",media_total); 
    if( write(atoi(argv[1]), str_notas, strlen(str_notas) + 1) != strlen(str_notas) + 1){
        perror("[PC] Error enviando la nota media. \n");
        free(gp_tabla_estudiantes);
        exit(EXIT_FAILURE);
    }

    if (kill(getppid(), SIGUSR2) == -1) {
        perror("[PB] Error enviando señal al manager. \n");
        free(gp_tabla_estudiantes);
        exit(EXIT_FAILURE);
    }

    sleep(5); /* Para poder probar Ctrl + C*/
    printf("[PC] Proceso terminado. \n");
    free(gp_tabla_estudiantes);
    return EXIT_SUCCESS;
}

void instalarManejador(){
    if(signal(SIGINT, manejador) == SIG_ERR){
        perror("[PC] No se pudo establecer el manejador de señales para SIGINT. \n");
        exit(EXIT_FAILURE);
    }  
}

void manejador(int signal){
    if (signal == SIGINT){
        printf("[PC] Terminando el proceso (SIGINT). \n");
        free(gp_tabla_estudiantes);
        exit(EXIT_SUCCESS);
    }
}