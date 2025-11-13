# Variáveis
CXX = g++
CXXFLAGS = -g -Wall -Wextra -Iinclude -std=c++11
SRCDIR = src
OBJDIR = obj
BINDIR = bin
EXEC = $(BINDIR)/tp2.out 

SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))

all: $(EXEC)

$(EXEC): $(OBJ)
	mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)/*.o $(BINDIR)/tp2.out