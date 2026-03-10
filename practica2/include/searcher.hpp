#include "searchResult.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Searcher{
    private:
        int id;
        int start;
        int finish;
        std::vector<SearchResult> results;
        std::string objective_word; //si mando ejecutar el codigo de otra clase no mantendria word (en caso de que fuese una variable global de main)
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
