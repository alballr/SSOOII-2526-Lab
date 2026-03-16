#include "searchResult.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

/************************************************************
 * Project        : Practica 2 de Sistemas Operativos II
 *
 * Program name   : searcher.hpp
 *
 * Author         : Alba Llorente
 *
 * Date created   : 02/03/2026
 *
 * Purpose        : Incluye las definiciones del codigo que ejecutará un hilo (buscar ocurrencias en sus lineas del texto)
 ************************************************************/

class Searcher{
    private:
        int id;
        int start;
        int finish;
        std::vector<SearchResult> results;
        std::string objective_word; 
        std::string filepath;
    
    public:
        Searcher(int id_, int start_, int finish_, std::string word_, std::string filepath_);
        void findOccurrences();
        std::vector<SearchResult> getResults();
        int getId() const;
        int getStart() const;
        int getFinish() const;
        std::vector<SearchResult> getResults() const;
};
