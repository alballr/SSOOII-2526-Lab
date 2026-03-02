#include "searchResult.h"
#include <vector>
#include <string>

class Searcher{
    private:
        int Id;
        int Start;
        int Finish;
        std::vector<SearchResult> results;
        std::string Word; //si mando ejecutar el codigo de otra clase no mantendria word (en caso de que fuese una variable global de main)
    
    public:
        Searcher(int id, int start, int finish, std::string word);
        void findOccurrences();
};