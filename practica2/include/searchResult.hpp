
// Minimal includes for std::string
#include <string>
/************************************************************
 * Project        : Practica 2 de Sistemas Operativos II
 *
 * Program name   : searchResult.hpp
 *
 * Author         : Alba Llorente
 *
 * Date created   : 02/03/2026
 *
 * Purpose        : incluye las definiciones de la estructura que engloba un resultado de la busqueda
 ************************************************************/
class SearchResult {
    private:
        int Line;
        std::string Previous;
        std::string Next;

    public:
        // Constructor
        SearchResult(int line, std::string previous, std::string next);

        // Getters
        int getLine() const;
        const std::string& getPrevious() const;
        const std::string& getNext() const;

};
