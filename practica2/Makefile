DIROBJ := obj
DIREXE := exec

CXX := g++
CPPFLAGS := -I.
CXXFLAGS := -Wall -Wextra -std=c++17
LDLIBS := -pthread -lm

TARGET := ssooiigle
SRCS := ssooiigle.cpp searcher.cpp searchResult.cpp
OBJS := $(addprefix $(DIROBJ)/,$(SRCS:.cpp=.o))

all: dirs $(DIREXE)/$(TARGET)

dirs:
	mkdir -p $(DIROBJ) $(DIREXE)

$(DIREXE)/$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDLIBS)

$(DIROBJ)/%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(DIROBJ) $(DIREXE)

execute: all
	./$(DIREXE)/$(TARGET)
