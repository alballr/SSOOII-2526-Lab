#include <iostream>
#include <sstream>
#include <thread>
#include <cstdlib>
#include <queue>
#include <fstream>
#include <vector>
#include <algorithm>  
#include "searcher.hpp"

/************************************************************
 * Project        : Practica 2 de Sistemas Operativos II
 *
 * Program name   : ssoiigle.cpp
 *
 * Author         : Alba Llorente
 *
 * Date created   : 02/03/2026
 *
 * Purpose        : Código del proceso principal, maneja los argumentos y crea y gestiona los hilos.
 ************************************************************/

// funciones de soporte
void parseArgv(int argc, char * argv[], int *p_threads);
int getNumLines();
void printOcurrences(Searcher searcher);

// variables globales
std::string g_word;
std::string g_filepath;

int main(int argc, char* argv[]){

    int n_threads, start, finish, n_lines, block_size;
    std::vector<Searcher> searchers;
    std::vector<std::thread> threads;

    parseArgv(argc, argv, &n_threads);

    if((n_lines = getNumLines()) == 0){
        std:: cout << "El archivo está vacio" << std::endl;
        exit(EXIT_SUCCESS);
    }
    n_threads = std::min(n_threads, n_lines); //por si hay mas hilos que lineas tiene el archivo
    block_size = n_lines/n_threads;
    for(int thread = 1; thread <= n_threads; thread++){
        start = (thread - 1) * block_size + 1; 
        finish = (thread == n_threads) ? n_lines : thread * block_size;
        searchers.emplace_back(thread, start, finish, g_word, g_filepath);
        threads.emplace_back(&Searcher::findOccurrences, &searchers[thread - 1]);
    }
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    for( Searcher searcher : searchers){
        printOcurrences(searcher);
    }
}

void parseArgv(int argc, char * argv[], int *p_threads){
    if(argc != 4){
        std::cerr << "Error en los argumentos \n";
        exit(EXIT_FAILURE);
    }
    std::ifstream test(argv[1]);
    if (!test.is_open()) {
        std::cerr << "El archivo indicado en argv[1] no existe o no se puede abrir\n";
        exit(EXIT_FAILURE);
    }
    g_filepath = argv[1];

    g_word = argv[2];
    if((*p_threads = atoi(argv[3])) == 0 ){
        std::cerr << "Numero de hilos inválido \n";
        exit(EXIT_FAILURE);
    }
}

int getNumLines(){
    std::string line;
    int n_lines = 0;
    std::ifstream file(g_filepath);
    if (!file.is_open()) {
        return 0;
    }
    while(std::getline(file, line)){
        n_lines++;
    }
    return n_lines;
}

void printOcurrences(Searcher searcher){
    std::vector<SearchResult> results = searcher.getResults();
    for(size_t i = 0; i < results.size(); i++){
        std::cout << "[Hilo" << searcher.getId() << " inicio:"<<searcher.getStart() << " – final:" << searcher.getFinish() << 
        "] :: línea " << results[i].getLine() << " :: …" << results[i].getPrevious() << " " << g_word << " "  << results[i].getNext() << " …" << std::endl;
    }
}
