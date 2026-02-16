#include <stdio.h>
#include <stdlib.h>
#include <definitions.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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
 *
 * Revision History :
 *
 * Date        Author    Ref   Revision
 * 
 *
 ************************************************************/
#define MAX_PATH 4096
#define MAX_STR 512
#define NOTAS_FILE "notas.txt"
#define NOTAS_STR "La nota que debes obtener en este nuevo examen para superar la prueba es"

int main(int argc, char *argv){
    int num_estudiantes = 0;
    int i, nota, media_total, fd = 0; 
    char path_notas[MAX_PATH];
    struct FichaEstudiante *p_tabla_estudiantes = NULL; /*tabla que almacena los datos de todos los estudiantes*/
    char str_notas[MAX_STR];

    if(read(0, &num_estudiantes, sizeof(int)) <= 0) {
        perror("[PC] Error leyendo numero de estudiantes\n");
        exit(EXIT_FAILURE);
    }
    if((p_tabla_estudiantes = malloc(num_estudiantes * sizeof(struct FichaEstudiante))) == NULL){
        perror("[PC] Error alocando espacio para las fichas de Estudiantes\n");
        exit(EXIT_FAILURE);
    }

    if( read(0,p_tabla_estudiantes,num_estudiantes * sizeof(struct FichaEstudiante)) <= 0 ){
        perror("[PC] Error leyendo lista de estudiantes\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < num_estudiantes; i++) {
        snprintf(path_notas, MAX_PATH, "%s/%s/%s", STU_DIR_PATH, p_tabla_estudiantes[i].dni,NOTAS_FILE);
        nota = 2 * 5 - p_tabla_estudiantes[i].nota;
        media_total = (nota + media_total) / 2;

        if(fd = open(path_notas, O_WRONLY | O_CREAT | O_TRUNC, 0644) == -1){
            perror("[PC] Error creando el archivo de notas\n");
            exit(EXIT_FAILURE);
        }
        snprintf(str_notas, MAX_STR, "%s %d",NOTAS_STR, nota);
        if( write(fd,str_notas,strlen(str_notas) + 1) != strlen(str_notas) + 1){
            perror("[PC] Error escribiendo en el archivo de notas\n");
            exit(EXIT_FAILURE);
        }
    }

    snprintf(str_notas, MAX_STR,"%d",media_total); /*se reutiliza el str usado para escribir las notas*/
    if( write(atoi(argv[1]), str_notas, strlen(str_notas) + 1) != strlen(str_notas) + 1){
        perror("[PC] Error enviando la nota media\n");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}
