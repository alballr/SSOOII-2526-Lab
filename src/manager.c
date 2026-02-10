#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
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
    pid_t pid_A;
    int status; //Status para el wait
    int tuberia_A[2]; //Tuberia que conecta el manager con el proceso A
    int tuberia_B[2]; //Tuberia que conecta el manager con el proceso B
    int tuberia_C[2]; //Tuberia que conecta el manager con el proceso C

    crearFichas();

    if(pipe(tuberia_A) == -1){
        perror("[MANAGER] Error estableciendo la tubería para el proceso A");
        return EXIT_FAILURE;
    }
    switch(pid_A=fork()){
        case -1:
            fprintf(stderr,"[MANAGER] Error creando el proceso A \n");
            return EXIT_FAILURE;
        case 0:
            close(tuberia_A[ESCRITURA]);    
            dup2(tuberia_A[LECTURA], 0);     
            close(tuberia_A[LECTURA]);

            execl("./exec/pa","pa",NULL);
             
            fprintf(stderr,"[MANAGER] Error ejecutando el código de el proceso A\n");
            exit(EXIT_FAILURE);
        default:
            close(tuberia_A[LECTURA]);
            write(tuberia_A[ESCRITURA], &g_numEstudiantes, sizeof(int));
            write(tuberia_A[ESCRITURA], g_Estudiantes, g_numEstudiantes * sizeof(struct FichaEstudiante));
            close(tuberia_A[ESCRITURA]);
    }
    if(waitpid(pid_A,&status) == -1){
        fprintf(stderr, "[MANAGER] Error esperando al proceso A \n");
        return EXIT_FAILURE;
    }
    }

/************ otros métodos ************/
void crearFichas(){
    FILE *fd;
    char dni[32];
    char grupo;
    int nota;
    char path[512];
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
