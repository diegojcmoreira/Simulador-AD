# Name of the project
PROJ_NAME=SimuladorAD

# .c files
C_SOURCE=$(wildcard ./src/*.c ./src/alglib/*.cpp)
 
# .h files
H_SOURCE=$(wildcard ./src/*.h ./src/alglib/*.h)

# Object files
OBJ=$(subst .c*,.o,$(subst source,objects,$(C_SOURCE)))

# Compiler
CC=g++

# Flags for compiler
CC_FLAGS=-c -W -Wall -ansi -pedantic -o

RM = rm -rf

#
# Compilation and linking
#
all: $(PROJ_NAME)


$(PROJ_NAME): $(OBJ)
	$(CC) -o $@ $^
		
./objects/%.o: ./src/%.c ./src/alglib/%.cpp ./src/%.h ./src/alglib/%.h

	$(CC) -o $@ $< $(CC_FLAGS)

./objects/simulation.o: ./src/simulation.c $(H_SOURCE)
	$(CC) -o $@ $< $(CC_FLAGS)

clean:
	$(RM) ./objects/*.o $(PROJ_NAME) *~