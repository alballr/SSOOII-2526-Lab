DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/

CXX := g++
CPPFLAGS := -I. -I$(DIRHEA)
CXXFLAGS := -Wall -Wextra -std=c++17
LDLIBS := -pthread -lm

TARGET := ssooiigle
SRCS := ssooiigle.cpp searcher.cpp searchResult.cpp
SRCS := $(addprefix $(DIRSRC),$(SRCS))
OBJS := $(addprefix $(DIROBJ),$(notdir $(SRCS:.cpp=.o)))

all: dirs $(DIREXE)$(TARGET)

dirs:
	mkdir -p $(DIROBJ) $(DIREXE)

$(DIREXE)$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDLIBS)

$(DIROBJ)%.o: $(DIRSRC)%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(DIROBJ) $(DIREXE)

execute: all
	./$(DIREXE)$(TARGET) $(ARGS)

make test: all
	./$(DIREXE)$(TARGET) libros/17_LEYES_DEL_TRABAJO_EN_EQUIPO.txt ley 4
