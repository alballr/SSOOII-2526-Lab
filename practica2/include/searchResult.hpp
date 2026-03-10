
// Minimal includes for std::string
#include <string>

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
