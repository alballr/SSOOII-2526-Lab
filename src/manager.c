#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "definitions.h"
#include <signal.h>
#include <errno.h>

#define NUM_PROCESOS 3 /*Numero de procesos del programa (pa, pb y pc)*/
#define CAPACIDAD_INI 10 /*Capacidad inicial de la estructura de estudiantes*/

/************************************************************
 * Project        : Practica 1 de Sistemas Operativos II
 *
 * Program name   : manager.c
 *
 * Author         : Alba Llorente
 *
 * Date created   : 10/02/2026
 *
 * Purpose        : Proceso encargado de la creación y manejo de los pdem
 ************************************************************/

struct FichaEstudiante *g_Estudiantes = NULL; /*Tabla de fichas de estudiantes */
int g_numEstudiantes = 0; /*Número de estudiantes*/ 
pid_t g_pids[NUM_PROCESOS]; /*almacena los pid de los procesos hijo*/

/* Métodos de apoyo */
void crearFichas();
void crearTuberias(int* tuberia_A, int* tuberia_B, int* tuberia_C, int* tuberia_C2);
void enviarEstudiantes(int *tuberia);
void instalarManejador();
void manejador();
void liberarRecursos();
void finalizarProcesos();
void esperarProcesos();


/************ Función Main ************/
int main(int argc, char *argv[]){
    int tuberia_A[2]; /*Tuberia que conecta el manager con el proceso A*/
    int tuberia_B[2]; /*Tuberia que conecta el manager con el proceso B*/
    int tuberia_C[2]; /*Tuberia que conecta el manager con el proceso C*/
    int tuberia_C2[2];/*Segunda tuberia que conecta el manager con el proceso C*/
    char str_tuberiaC[256]; /*String de la tubería de C*/

    instalarManejador();
    crearFichas();
    crearTuberias(tuberia_A, tuberia_B, tuberia_C, tuberia_C2);    

    /****** Creacion de PA *******/
    switch(g_pids[0]=fork()){
        case -1:
            perror("[MANAGER] Error creando el proceso A \n");
            return EXIT_FAILURE;
        case 0:
            close(tuberia_A[ESCRITURA]);    
            dup2(tuberia_A[LECTURA], STDIN_FILENO);     
            close(tuberia_A[LECTURA]);
            execl("./exec/pa","pa",NULL);
             
            fprintf(stderr,"Error ejecutando el código de el proceso A: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
    }
    printf("[MANAGER] Proceso A creado \n");
    enviarEstudiantes(tuberia_A);

    if(wait(NULL) == -1){
        fprintf(stderr, "[MANAGER] Error esperando al proceso A \n");
        return EXIT_FAILURE;
    }
    g_pids[0] = 0; /*Para marcar que este proceso ya finalizó.*/
    sleep(3);
    /****** Creacion de PB *******/
    switch(g_pids[1] = fork()){
       case -1:
            perror("[MANAGER] Error creando el proceso B \n");
            return EXIT_FAILURE;
        case 0:
            close(tuberia_B[ESCRITURA]);    
            dup2(tuberia_B[LECTURA], STDIN_FILENO);     
            close(tuberia_B[LECTURA]);
            execl("./exec/pb","pb",NULL);
             
            fprintf(stderr," Error ejecutando el código de el proceso B %s\n", strerror(errno));
            exit(EXIT_FAILURE);       
    }
    enviarEstudiantes(tuberia_B);
    printf("[MANAGER] Proceso B creado \n");

     /****** Creacion de PC *******/
    sprintf(str_tuberiaC, "%d", tuberia_C2[ESCRITURA]);

    switch(g_pids[2] = fork()){
       case -1:
            perror("[MANAGER] Error creando el proceso C \n");
            return EXIT_FAILURE;
        case 0:
            close(tuberia_C[ESCRITURA]);    
            dup2(tuberia_C[LECTURA], STDIN_FILENO);     
            close(tuberia_C[LECTURA]);
            execl("./exec/pc", "pc", str_tuberiaC, NULL);
             
            fprintf(stderr," Error ejecutando el código de el proceso C %s\n", strerror(errno));
            exit(EXIT_FAILURE);       
    }
    enviarEstudiantes(tuberia_C);
    printf("[MANAGER] Proceso C creado\n");

     /****** Esperar a finalización *******/
    esperarProcesos();
    printf("[MANAGER] Programa terminado \n");
    return EXIT_SUCCESS;
    }

/************ otros métodos ************/
void crearFichas(){
    FILE *fd;
    char dni[32];
    char grupo;
    int nota;
    int capacidad = CAPACIDAD_INI;

    if((fd = fopen(STU_FILE_PATH,"r"))== NULL){
        perror("[MANAGER] Error intentando leer el archivo 'estudiantes.txt'");
        exit(EXIT_FAILURE);
    }

    if((g_Estudiantes = malloc(capacidad * sizeof(struct FichaEstudiante))) == NULL){
        perror("[MANAGER] Error alocando espacio para las fichas de Estudiantes");
        exit(EXIT_FAILURE);
    }

    while (fscanf(fd, "%31s %c %d", dni, &grupo, &nota) == 3) {
        if (g_numEstudiantes >= capacidad) {
            capacidad *= 2;
            struct FichaEstudiante *tmp = realloc(g_Estudiantes, capacidad * sizeof(struct FichaEstudiante));
            if (tmp == NULL) {
                perror("[MANAGER] Error realocando espacio para las fichas de Estudiantes");
                free(g_Estudiantes);
                fclose(fd);
                exit(EXIT_FAILURE);
            }
            g_Estudiantes = tmp;
        }
        strncpy(g_Estudiantes[g_numEstudiantes].dni, dni, sizeof(g_Estudiantes[g_numEstudiantes].dni)-1);
        g_Estudiantes[g_numEstudiantes].dni[sizeof(g_Estudiantes[g_numEstudiantes].dni)-1] = '\0';
        g_Estudiantes[g_numEstudiantes].grupo = grupo;
        g_Estudiantes[g_numEstudiantes].nota = nota;
        g_numEstudiantes++;
    }
    fclose(fd);
}

void crearTuberias(int* tuberia_A, int* tuberia_B, int* tuberia_C, int* tuberia_C2){
    if(pipe(tuberia_A) == -1){
        perror("[MANAGER] Error estableciendo la tubería para el proceso A");
        exit(EXIT_FAILURE);
    }
    if(pipe(tuberia_B) == -1){
        perror("[MANAGER] Error estableciendo la tubería para el proceso B");
        exit(EXIT_FAILURE);
    }
    if(pipe(tuberia_C) == -1 || pipe(tuberia_C2) == -1){
        perror("[MANAGER] Error estableciendo las tuberías para el proceso C");
        exit(EXIT_FAILURE);
    }
}

void enviarEstudiantes(int* tuberia){
    close(tuberia[LECTURA]);
    write(tuberia[ESCRITURA], &g_numEstudiantes, sizeof(int));
    write(tuberia[ESCRITURA], g_Estudiantes, g_numEstudiantes * sizeof(struct FichaEstudiante));
    close(tuberia[ESCRITURA]);
}

void instalarManejador(){
    if(signal(SIGINT, manejador) == SIG_ERR){
        perror("[MANAGER] No se pudo establecer el manejador de señales para SIGINT\n");
        exit(EXIT_FAILURE);
    }  
}

void manejador(int signal){
    if (signal == SIGINT){
        printf("[MANAGER] Terminando el programa (Ctrl + C) \n");
        finalizarProcesos();
        liberarRecursos();
        exit(EXIT_SUCCESS);
    }
}

void finalizarProcesos(){
    int i;
    printf("[MANAGER] Finalizando los procesos en ejecución...\n");
    for(i = 0; i < NUM_PROCESOS; i++){
        if (g_pids[i] > 0) {
            if (kill(g_pids[i], SIGINT) == -1) {
                fprintf(stderr, "[MANAGER] Error al terminar el proceso %c (pid %d)\n", 'A' + i, g_pids[i]);
            }
        }
    }
}

void liberarRecursos(){
    free(g_Estudiantes);

    switch(fork()){
        case -1:
            perror("[MANAGER] Error creando el proceso D \n");
            exit(EXIT_FAILURE);
        case 0:
            execl("/bin/rm","rm","-rf","./estudiantes/*", NULL);
            
            fprintf(stderr,"Error ejecutando el código de el proceso D\n");
            exit(EXIT_FAILURE);
    }
}

void esperarProcesos(){
    int i, pid;
    for(i=0; i<2;i++){
       pid = wait(NULL);
       for(i=1;i<3;i++){
        if (g_pids[i]==pid){
            g_pids[i] = 0;
        }
       } 
    }
}