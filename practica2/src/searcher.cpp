#include "searcher.hpp"

/************************************************************
 * Project        : Practica 2 de Sistemas Operativos II
 *
 * Program name   : searcher.cpp
 *
 * Author         : Alba Llorente
 *
 * Date created   : 02/03/2026
 *
 * Purpose        : Engloba el codigo que ejecutará un hilo (buscar ocurrencias en sus lineas del texto)
 ************************************************************/

// Constructor
Searcher::Searcher(int id_, int start_, int finish_, std::string word_, std::string filepath_) 
: id(id_), start(start_), finish(finish_), objective_word(word_), filepath(filepath_) {} 

void Searcher::findOccurrences(){
    int n_line = 0;
    std::string line, current_word, previous_word, next_word;
    std::vector<std::string> line_words; // palabras separadas de cada linea
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return;
    }

    while(n_line < start){
        std::getline(file,line);
        n_line++;
    }

    for(n_line = start; n_line <= finish; n_line++){ 
        std::getline(file, line);
        std::stringstream ss(line);

        line_words.clear();
        while (ss >> current_word) {
            line_words.push_back(current_word);
        }
        for(size_t i = 0; i < line_words.size(); i++){
            if(line_words[i] == objective_word){
                previous_word = (i == 0) ? "_" : line_words[i - 1];
                next_word = (i + 1 >= line_words.size()) ? "_" : line_words[i + 1];
                results.emplace_back(n_line,previous_word,next_word);
            }
        }
    }      
}

// Getters
int Searcher::getId() const {
    return id;
}

int Searcher::getStart() const {
    return start;
}

int Searcher::getFinish() const {
    return finish;
}

std::vector<SearchResult> Searcher::getResults() {
    return results;
}
