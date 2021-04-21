CXX = g++
CXXFLAGS = 
LDFLAGS = 

SRC = random.cpp SGA_01.cpp SGA_Facade.cpp SGA_Kocierz.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = SGA

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

clean:
	rm -rf $(OBJ)