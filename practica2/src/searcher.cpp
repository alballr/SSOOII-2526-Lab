#include "searcher.hpp"
#include "inc.hpp"


// Constructor
Searcher::Searcher(int id_, int start_, int finish_, std::string word_, std::string filepath_) 
: id(id_), start(start_), finish(finish_), objective_word(word_), filepath(filepath_) {} 

void Searcher::findOccurrences(){
    int n_line;
    std::string line, current_w, previous_w, next_w;
    std::vector<std::string> line_words; // palabras separadas de cada linea
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return;
    }

    for(n_line = 0; n_line < start; n_line++){
        std::getline(file,line);
    }

    for(n_line = start; n_line <= finish; n_line++){ 
        std::getline(file, line);
        std::stringstream ss(line);

        line_words.clear();
        while (ss >> current_w) {
            line_words.push_back(current_w);
        }
        for(size_t i = 0; i < line_words.size(); i++){
            if(line_words[i] == objective_word){
                previous_w = (i == 0) ? "_" : line_words[i - 1];
                next_w = (i + 1 >= line_words.size()) ? "_" : line_words[i + 1];
                results.emplace_back(n_line,previous_w,next_w);
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

std::vector<SearchResult> Searcher::getResults() const {
    return results;
}

std::vector<SearchResult> Searcher::getResults() {
    return results;
}
