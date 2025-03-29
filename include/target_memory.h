#ifndef TARGET_MEMORY_H
#define TARGET_MEMORY_H

#include "semantic_analyzer.h" // Ensure MatrixIR is defined

// Memory layout structure holds base addresses for each operand.
typedef struct {
    unsigned int base_A;
    unsigned int base_B;
    unsigned int base_C;
    unsigned int base_V; // For vector (used in matrix-vector multiplication)
} MemoryLayout;

// Computes the memory layout for matrices based on IR dimensions.
// Assumes:
//   - Matrix A starts at address 0x0000,
//   - Matrix B (or vector V) immediately follows A,
//   - Matrix C is placed after B.
void compute_memory_layout(MatrixIR *ir, MemoryLayout *layout);

// Structure for a matrix memory lookup entry.
typedef struct {
    char identifier;          // For example: 'A', 'B', 'C', or 'V'
    unsigned int base_address;
} MatrixMemoryInfo;

// Lookup table accessor functions.
int get_matrix_memory_table_size(void);
const MatrixMemoryInfo * get_matrix_memory_info_by_index(int index);

#endif // TARGET_MEMORY_H
