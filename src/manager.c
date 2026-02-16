#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <definitions.h>

#define NUM_PROCESSES 3
#define INITIAL_SIZE 10


/************************************************************
 * Project        : Practica 1 de Sistemas Operativos II
 *
 * Program name   : manaer.c
 *
 * Author         : Alba Llorente
 *
 * Date created   : 10/02/2026
 *
 * Purpose        : Proceso encargado de la creación y manejo de los pdem
 *
 * Revision History :
 *
 * Date        Author    Ref   Revision
 * 10/02/2026  Alba   1      Paso de la lectura del archivo estudiantes.txt a manager.c
 *
 ************************************************************/

struct FichaEstudiante *g_Estudiantes = NULL; /*Tabla de fichas de estudiantes */
int g_numEstudiantes = 0; /*Número de estudiantes*/ 

/* Métodos de apoyo */
void crearFichas();

/************ Función Main ************/
int main(int argc, char *argv){
    pid_t pid_A, pid_B, pid_C;
    int status; /*Status para el wait*/
    int tuberia_A[2]; /*Tuberia que conecta el manager con el proceso A*/
    int tuberia_B[2]; /*Tuberia que conecta el manager con el proceso B*/
    int tuberia_C[2]; /*Tuberia que conecta el manager con el proceso C*/
    int tuberia_C2[2];/*Segunda tuberia que conecta el manager con el proceso C*/
    char str_tuberiaC[256]; /*String de la tubería de C*/

    crearFichas();
    crearTuberias(tuberia_A, tuberia_B, tuberia_C, tuberia_C2);    

    /****** Creacion de PA *******/
    switch(pid_A=fork()){
        case -1:
            perror("[MANAGER] Error creando el proceso A \n");
            return EXIT_FAILURE;
        case 0:
            close(tuberia_A[ESCRITURA]);    
            dup2(tuberia_A[LECTURA], STDIN_FILENO);     
            close(tuberia_A[LECTURA]);

            execl("./exec/pa","pa",NULL);
             
            fprintf(stderr,"[MANAGER] Error ejecutando el código de el proceso A\n");
            exit(EXIT_FAILURE);
    }
    enviarEstudiantes(tuberia_A);

    if(waitpid(pid_A,&status,0) == -1){
        fprintf(stderr, "[MANAGER] Error esperando al proceso A \n");
        return EXIT_FAILURE;
    }
    /****** Creacion de PB *******/
    switch(pid_B = fork()){
       case -1:
            perror("[MANAGER] Error creando el proceso B \n");
            return EXIT_FAILURE;
        case 0:
            close(tuberia_B[ESCRITURA]);    
            dup2(tuberia_B[LECTURA], STDIN_FILENO);     
            close(tuberia_B[LECTURA]);

            execl("./exec/pb","pb",NULL);
             
            fprintf(stderr,"[MANAGER] Error ejecutando el código de el proceso B\n");
            exit(EXIT_FAILURE);       
    }
    enviarEstudiantes(tuberia_B);

     /****** Creacion de PC *******/
    sprintf(str_tuberiaC, "%d", tuberia_C2[ESCRITURA]);

    switch(pid_B = fork()){
       case -1:
            perror("[MANAGER] Error creando el proceso C \n");
            return EXIT_FAILURE;
        case 0:
            close(tuberia_C[ESCRITURA]);    
            dup2(tuberia_C[LECTURA], STDIN_FILENO);     
            close(tuberia_C[LECTURA]);

            execl("./exec/pc", "pc", str_tuberiaC, NULL);
             
            fprintf(stderr,"[MANAGER] Error ejecutando el código de el proceso C\n");
            exit(EXIT_FAILURE);       
    }
    enviarEstudiantes(tuberia_C);
    
    return EXIT_SUCCESS;
    }

/************ otros métodos ************/
void crearFichas(){
    FILE *fd;
    char dni[32];
    char grupo;
    int nota;
    int capacidad = INITIAL_SIZE;

    if((fd = fopen(STU_FILE_PATH,"r"))== NULL){
        perror("[MANAGER] Error intentando leer el archivo 'estudiantes.txt'\n");
    }

    if((g_Estudiantes = malloc(capacidad * sizeof(struct FichaEstudiante))) == NULL){
        perror("[MANAGER] Error alocando espacio para las fichas de Estudiantes\n");
        exit(EXIT_FAILURE);
    }

    while (fscanf(fd,"%s %c %d", dni, &grupo, &nota) == 3) {
        strcpy(g_Estudiantes[g_numEstudiantes].dni,dni);
        g_Estudiantes[g_numEstudiantes].grupo = grupo;
        g_Estudiantes[g_numEstudiantes].nota = nota;
        g_numEstudiantes++;
    }
    fclose(fd);
}

void crearTuberias(int* tuberia_A, int* tuberia_B, int* tuberia_C, int* tuberia_C2){
    if(pipe(tuberia_A) == -1){
        perror("[MANAGER] Error estableciendo la tubería para el proceso A");
        return EXIT_FAILURE;
    }
    if(pipe(tuberia_B) == -1){
        perror("[MANAGER] Error estableciendo la tubería para el proceso B");
        return EXIT_FAILURE;
    }
    if(pipe(tuberia_C) == -1 || pipe(tuberia_C2)){
        perror("[MANAGER] Error estableciendo las tuberías para el proceso C");
        return EXIT_FAILURE;
    }
}

void enviarEstudiantes(int* tuberia){
    close(tuberia[LECTURA]);
    write(tuberia[ESCRITURA], &g_numEstudiantes, sizeof(int));
    write(tuberia[ESCRITURA], g_Estudiantes, g_numEstudiantes * sizeof(struct FichaEstudiante));
    close(tuberia[ESCRITURA]);
}
