#include "searchResult.hpp"
#include "definitions.hpp"

/************************************************************
 * Project        : Practica 2 de Sistemas Operativos II
 *
 * Program name   : searchResult.cpp
 *
 * Author         : Alba Llorente
 *
 * Date created   : 02/03/2026
 *
 * Purpose        : Estructura que engloba un resultado de la busqueda
 ************************************************************/
SearchResult::SearchResult(int line, std::string previous, std::string next)
    : Line(line), Previous(previous), Next(next) {}

int SearchResult::getLine() const {
    return Line;
}

const std::string& SearchResult::getPrevious() const {
    return Previous;
}

const std::string& SearchResult::getNext() const {
    return Next;
}
