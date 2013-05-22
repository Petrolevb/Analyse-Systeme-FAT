CC=g++
CCFLAGS=-g -Wall -Wextra -ansi -pedantic

SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
HEAD=$(wildcard *.hpp)

EXEC=bin/FS-Analysis

all: $(EXEC)

clean:
	@rm -f  $(OBJ)

distclean : clean
	@rm -f  $(EXEC)


$(EXEC): $(OBJ)
	@$(CC) $(OBJ) -o $(EXEC) 


%o: %cpp
	@$(CC) -o $@ -c $< $(CCFLAGS)

