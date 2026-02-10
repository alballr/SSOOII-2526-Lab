#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

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
 *
 ************************************************************/

int main(int argc, char *argv[]){
    FILE *fd;
    char dni[32];
    char grupo;
    int nota;
    char path[512];

    if((fd=fopen("estudiantes.txt","r"))== NULL){
        fprintf(stderr,"Error intentando leer el archivo 'estudiantes.txt'\n");
    }
    
   while (fscanf(fd,"%s %c %d", dni, &grupo, &nota) == 3) {
        snprintf(path,512,"./estudiantes/%s",dni);
        mkdir(path,0775);
    } 
    
    return EXIT_SUCCESS;
}
