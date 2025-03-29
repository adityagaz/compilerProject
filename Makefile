CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude -fopenmp

# Compiler source files and objects
COMPILER_SRCS = src/compiler_main.c src/syntax_parser.c src/semantic_analyzer.c src/ir_optimizer.c src/target_memory.c src/isa_generator.c src/isa_encoding.c
COMPILER_OBJS = $(COMPILER_SRCS:.c=.o)
COMPILER_TARGET = matrix_compiler

# --- NEW: Print Tables specific files ---
PRINT_TABLES_SRC = src/print_tables.c
# Objects needed by print_tables (contains table data/accessors)
PRINT_TABLES_DEP_OBJS = src/isa_encoding.o src/target_memory.o
PRINT_TABLES_OBJ = $(PRINT_TABLES_SRC:.c=.o)
PRINT_TABLES_TARGET = print_lookup_tables
# --- END NEW ---

# Default target builds the main compiler
all: $(COMPILER_TARGET)

$(COMPILER_TARGET): $(COMPILER_OBJS)
	$(CC) $(CFLAGS) -o $@ $(COMPILER_OBJS)

# --- NEW: Rule to build the print_tables executable ---
# It depends on its own object file AND the object files containing the tables/accessors
$(PRINT_TABLES_TARGET): $(PRINT_TABLES_OBJ) $(PRINT_TABLES_DEP_OBJS)
	$(CC) $(CFLAGS) -o $@ $(PRINT_TABLES_OBJ) $(PRINT_TABLES_DEP_OBJS)

# Add a target to easily build the printer
print_tables: $(PRINT_TABLES_TARGET)
# --- END NEW ---


# Generic rule to compile any .c in src to .o in src
# Make sure object files required by BOTH targets are built
src/%.o: src/%.c include/*.h Makefile
	$(CC) $(CFLAGS) -c -o $@ $<


# Updated clean rule
clean:
	rm -f $(COMPILER_OBJS) $(COMPILER_TARGET) $(PRINT_TABLES_OBJ) $(PRINT_TABLES_TARGET) src/*.o output.isa
