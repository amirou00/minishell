CC=gcc
CFLAGS=-std=c11 -Wpedantic -Wall -Wextra -fsanitize=address -g
LDLIBS=-lm
RM=rm-f
OBJ=main.o minishell.o 
EXE=minishell

$(EXE):$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXE) $(LDLIBS)

main.o:main.c minishell.h 
	$(CC) $(CFLAGS) -c main.c
minishell.o:minishell.c minishell.h
	$(CC) $(CFLAGS) -c minishell.c

clean:
	$(RM) $(OBJ) $(EXE)