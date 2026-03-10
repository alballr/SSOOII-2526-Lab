#include "searchResult.hpp"
#include "inc.hpp"

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
