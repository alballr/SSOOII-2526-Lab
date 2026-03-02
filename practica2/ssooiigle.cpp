#include <iostream>
#include <sstream>
#include <thread>
#include <cstdlib>
#include <queue>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>  
#include "searcher.h"

// funciones de soporte
void parseArgv(int argc, char * argv[], std::ifstream *file, std::string *word, int *n_threads);
int getNumLines(std::ifstream *file);

int main(int argc, char* argv[]){
    std::ifstream file;
    std::string word;
    int n_threads, start, finish, n_lines, block_size;
    std::vector<Searcher> searchers;
    std::vector<std::thread> threads;

    parseArgv(argc, argv,&file, &word, &n_threads);

    n_lines = getNumLines(&file);
    block_size = n_lines/n_threads;
    for(int thread = 1; thread <= n_threads; thread++){
        start = thread * block_size ; // lines starting on 1.
        finish = (thread == n_threads -1) ? n_lines : start + block_size;
        searchers.emplace_back(thread, start, finish, word);
        threads.emplace_back(&Searcher::findOccurrences, &searchers[thread - 1]);
    }
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

}

void parseArgv(int argc, char * argv[], std::ifstream *file, std::string *word, int *n_threads){
    if(argc !=4){
        std::cerr << "Error en el número \n";
        exit(EXIT_FAILURE);
    }
    (*file).open(argv[1]);
    if (! (*file).is_open()) {
        std::cerr << "El archivo indicado en argv[1] no existe o no se puede abrir\n";
        exit(EXIT_FAILURE);
    }

    *word = argv[2];
    if((*n_threads = atoi(argv[3])) == 0 ){
        std::cerr << "Numero de hilos inválido \n";
        exit(EXIT_FAILURE);
    }
}

int getNumLines(std::ifstream *file){
    std::string line;
    int n_lines = 0;
    while(std::getline(*file, line)){
        n_lines++;
    }
    (*file).seekg(0);
    return n_lines;
}
