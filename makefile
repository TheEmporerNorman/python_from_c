#OBJS specifies which files to compile as part of the project
OBJS = main.c
 
#CC specifies which compiler we're using
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -Ofast -fno-finite-math-only -march=native -lm -lpython3.6m -L/user/lib/python3.6/config -fopenmp -w -g -Wall -Wextra -Wconversion -pedantic -std=gnu11

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lm

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = python_from_c.out

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME) 
