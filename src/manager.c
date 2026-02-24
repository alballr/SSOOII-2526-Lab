#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>
#include "definitions.h"

#define BUFFER_SIZE 4096
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

 /* variables globales*/
struct FichaEstudiante *g_Estudiantes = NULL; /*Tabla de fichas de estudiantes */
int g_numEstudiantes = 0; /*Número de estudiantes*/ 
pid_t g_pids[NUM_PROCESOS]; /*almacena los pid de los procesos hijo*/
pid_t g_pid_daemon; /* pid del proceso de backup*/
struct timeval g_fin_B, g_fin_C, g_tiempo_ini1, g_tiempo_ini2; 
double g_tiempo_B, g_tiempo_C;
FILE *g_log_file; 

/* Métodos de apoyo */
void crearFichas();
void crearTuberias(int* tuberia_A, int* tuberia_B, int* tuberia_C, int* tuberia_C2);
void enviarEstudiantes(int *tuberia);
void instalarManejador();
void manejador();
void liberarRecursos();
void finalizarProcesos();
void esperarProcesos();
void crearDaemon();
void ejecutarBackup();
void manejadorDaemon();

/************ Función Main ************/
int main(int argc, char *argv[]){
    int tuberia_A[2]; /*Tuberia que conecta el manager con el proceso A*/
    int tuberia_B[2]; /*Tuberia que conecta el manager con el proceso B*/
    int tuberia_C[2]; /*Tuberia que conecta el manager con el proceso C*/
    int tuberia_C2[2];/*Segunda tuberia que conecta el manager con el proceso C*/
    char str_tuberiaC[256], buffer[BUFFER_SIZE]; /*String de la tubería de C*/
    double tiempo_A, nota_media;
    struct timeval fin_A;

    crearDaemon();
    
    if ((g_log_file=fopen("log.txt", "w")) == NULL) {
        perror("[MANAGER] Error al crear el archivo log.txt. \n");
        return EXIT_FAILURE;
    }
    fprintf(g_log_file, "******** Log del sistema ********\n");

    instalarManejador();
    crearFichas();
    crearTuberias(tuberia_A, tuberia_B, tuberia_C, tuberia_C2);    

    gettimeofday(&g_tiempo_ini1, NULL);
    /****** Creacion de PA *******/
    switch(g_pids[0]=fork()){
        case -1:
            perror("[MANAGER] Error creando el proceso A \n");
            free(g_Estudiantes);
            return EXIT_FAILURE;
        case 0:
            close(tuberia_A[ESCRITURA]);    
            dup2(tuberia_A[LECTURA], STDIN_FILENO);     
            close(tuberia_A[LECTURA]);
            execl("./exec/pa","pa",NULL);
             
            fprintf(stderr,"Error ejecutando el código de el proceso A: %s\n", strerror(errno));
            free(g_Estudiantes);
            exit(EXIT_FAILURE);
    }
    printf("[MANAGER] Proceso A creado \n");
    enviarEstudiantes(tuberia_A);

    if(wait(NULL) == -1){
        fprintf(stderr, "[MANAGER] Error esperando al proceso A \n");
        liberarRecursos();
        return EXIT_FAILURE;
    }
    gettimeofday(&fin_A, NULL);
    tiempo_A = (fin_A.tv_sec - g_tiempo_ini1.tv_sec) * 1000.0;      
    tiempo_A += (fin_A.tv_usec - g_tiempo_ini1.tv_usec) / 1000.0;   
    g_pids[0] = 0; /*Para marcar que este proceso ya finalizó.*/
    fprintf(g_log_file, "Creación de directorios finalizada (tiempo invertido: %.3fms).\n", tiempo_A);
    sleep(3); /* Para poder probar el Ctrl + C */

    gettimeofday(&g_tiempo_ini1, NULL); /*Pasa a medir el tiempo para B*/
    /****** Creacion de PB *******/
    switch(g_pids[1] = fork()){
       case -1:
            perror("[MANAGER] Error creando el proceso B. \n");
            liberarRecursos();
            return EXIT_FAILURE;
        case 0:
            close(tuberia_B[ESCRITURA]);    
            dup2(tuberia_B[LECTURA], STDIN_FILENO);     
            close(tuberia_B[LECTURA]);
            execl("./exec/pb","pb",NULL);
             
            fprintf(stderr," Error ejecutando el código de el proceso B. %s\n", strerror(errno));
            liberarRecursos();
            exit(EXIT_FAILURE);       
    }
    enviarEstudiantes(tuberia_B);
    printf("[MANAGER] Proceso B creado. \n");

    gettimeofday(&g_tiempo_ini2, NULL); /*Mide el tiempo para C*/

    /****** Creacion de PC *******/
    sprintf(str_tuberiaC, "%d", tuberia_C2[ESCRITURA]);

    switch(g_pids[2] = fork()){
       case -1:
            perror("[MANAGER] Error creando el proceso C. \n");
            liberarRecursos();
            return EXIT_FAILURE;
        case 0:
            close(tuberia_C[ESCRITURA]);    
            dup2(tuberia_C[LECTURA], STDIN_FILENO);     
            close(tuberia_C[LECTURA]);
            execl("./exec/pc", "pc", str_tuberiaC, NULL);
             
            fprintf(stderr," Error ejecutando el código de el proceso C. %s\n", strerror(errno));
            liberarRecursos();
            exit(EXIT_FAILURE);       
    }
    enviarEstudiantes(tuberia_C);
    printf("[MANAGER] Proceso C creado. \n");

     /****** Esperar a finalización *******/
    esperarProcesos();

    if(read(tuberia_C2[LECTURA], buffer, BUFFER_SIZE) <= 0) {
        perror("[MANAGER] Error leyendo la nota media \n");
        exit(EXIT_FAILURE);
    }
    nota_media = atof(buffer);
    fprintf(g_log_file, "La nota media de la clase es: %.2f\n", nota_media);

    kill(g_pid_daemon, SIGTERM); /* para evitar que siga ejecutando con el programa terminado*/
    printf("[MANAGER] Programa terminado \n");
    fprintf(g_log_file, "FIN DE PROGRAMA\n");
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
        free(g_Estudiantes);
        exit(EXIT_FAILURE);
    }
    if(pipe(tuberia_B) == -1){
        perror("[MANAGER] Error estableciendo la tubería para el proceso B");
        free(g_Estudiantes);
        exit(EXIT_FAILURE);
    }
    if(pipe(tuberia_C) == -1 || pipe(tuberia_C2) == -1){
        perror("[MANAGER] Error estableciendo las tuberías para el proceso C");
        free(g_Estudiantes);
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
    if(signal(SIGUSR1, manejador) == SIG_ERR){
        perror("[MANAGER] No se pudo establecer el manejador de señales para SIGUSR1\n");
        exit(EXIT_FAILURE);
    }
    if(signal(SIGUSR2, manejador) == SIG_ERR){
        perror("[MANAGER] No se pudo establecer el manejador de señales para SIGUSR2\n");
        exit(EXIT_FAILURE);
    } 
}

void manejador(int signal){
    switch(signal){
        case SIGINT:
            printf("[MANAGER] Terminando el programa (Ctrl + C) \n");
            fprintf(g_log_file, "El usuario interrupió la ejecución del programa (Ctrl + C) \n");
            finalizarProcesos();
            liberarRecursos();
            exit(EXIT_SUCCESS);
        case SIGUSR1: /*enviada por proceso B*/
            gettimeofday(&g_fin_B, NULL);
            g_tiempo_B = (g_fin_B.tv_sec - g_tiempo_ini1.tv_sec) * 1000.0;      
            g_tiempo_B += (g_fin_B.tv_usec - g_tiempo_ini1.tv_usec) / 1000.0;
            fprintf(g_log_file, "Copia de modelos de examen, finalizada (tiempo invertido: %.3fms).\n", g_tiempo_B);
            break;
        case SIGUSR2: /*enviada por proceso C*/
            gettimeofday(&g_fin_C, NULL);
            g_tiempo_C = (g_fin_C.tv_sec - g_tiempo_ini2.tv_sec) * 1000.0;      
            g_tiempo_C += (g_fin_C.tv_usec - g_tiempo_ini2.tv_usec) / 1000.0; 
            fprintf(g_log_file, "Creación de archivos con nota necesaria para alcanzar la nota de corte, finalizada (tiempo invertido: %.3fms).\n", g_tiempo_C);
    } 
}

void finalizarProcesos(){
    int i;
    printf("[MANAGER] Finalizando los procesos en ejecución...\n");
    kill(g_pid_daemon, SIGTERM);
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
    int finished = 0;
    while (finished < 2) {
        pid = wait(NULL);
        for (i = 0; i < NUM_PROCESOS; ++i) {
            if (g_pids[i] == pid) {
                g_pids[i] = 0;
                finished++;
                break;
            }
        }
    }
}

void crearDaemon() {
    switch(g_pid_daemon = fork()){
        case -1:
            perror("[MANAGER] Error creando el daemon");
            exit(EXIT_FAILURE);
        case  0:
            if (setsid() < 0) {
            exit(EXIT_FAILURE);
            }
            signal(SIGTERM, manejadorDaemon);
            while (1) {
                ejecutarBackup();
                sleep(60); 
            }
        default:
            printf("[MANAGER] Daemon de backup creado (pid %d)\n", g_pid_daemon);      
    }  
}

void ejecutarBackup(){
    if(system("mkdir -p ./backup") == -1){
        perror("[BACKUP] Error ejecutando mkdir");
        exit(EXIT_FAILURE);
    }
    if(system("cp -r ./estudiantes ./backup/") == -1){
        perror("[BACKUP] Error ejecutando mkdir");
        exit(EXIT_FAILURE);
    }
    printf("[BACKUP] Backup realizado correctamente\n");
}

void manejadorDaemon(int sig) {
    if (sig == SIGTERM) {
        printf("[DAEMON] Finalizando daemon de backup...\n");
        exit(EXIT_SUCCESS);
    }
}