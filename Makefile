########################################
##
## Makefile
## LINUX compilation
##
##############################################


#FLAGS
C++FLAG = -g -std=c++14

MATH_LIBS = -lm

EXEC_DIR=.


.cc.o:
	g++ $(C++FLAG) $(INCLUDES)  -c $< -o $@


#Including
INCLUDES=  -I.

#-->All libraries (without LEDA)
LIBS_ALL =  -L/usr/lib -L/usr/local/lib

# main maze program
Cpp_OBJ=image.o disjoint_set.o maze.o utility_methods.o create_maze.o
PROGRAM_NAME=create_maze
# program to show how the cells are arranged and indexed
# Each row and column of white cells are indexed.
Cpp_OBJ1=image.o disjoint_set.o maze.o utility_methods.o create_grid.o
PROGRAM_NAME1=create_grid

$(PROGRAM_NAME): $(Cpp_OBJ)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ) $(INCLUDES) $(LIBS_ALL)
$(PROGRAM_NAME1): $(Cpp_OBJ1)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(Cpp_OBJ1) $(INCLUDES) $(LIBS_ALL)


all:
	make $(PROGRAM_NAME)
	make $(PROGRAM_NAME1)


clean:
	(rm -f *.o *.h.gch;)

(:
