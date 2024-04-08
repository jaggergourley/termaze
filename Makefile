# Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -Wextra -pedantic -std=c11

# Source Files
SRC = generate.c pathfind.c termaze.c

# Header Files
HDRS = generate.h pathfind.h termaze.h

# Object Files
OBJS = $(SRC:.c=.o)

# Executable Name
EXE = termaze

#Default Target
all: $(EXE)

# Compile Source Files Into Object Files
%.o: %.c $(HDRS) 
	$(CC) $(CFLAGS) -c $< -o $@

# Link Object Files Into Executable
$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXE)

# Clean Target
clean:
	rm -f $(EXE) $(OBJS)
