#include "target_memory.h"
#include "stdio.h"
// Define a static lookup table for matrix memory info.
// These sample base addresses should match your actual layout.
static MatrixMemoryInfo matrix_memory_table[] = {
    {'A', 0x0000},
    {'B', 0x0100},
    {'C', 0x0200},
    {'V', 0x0100}  // For matrix-vector multiplication.
};

int get_matrix_memory_table_size(void) {
    return sizeof(matrix_memory_table) / sizeof(matrix_memory_table[0]);
}

const MatrixMemoryInfo * get_matrix_memory_info_by_index(int index) {
    if (index < 0 || index >= get_matrix_memory_table_size())
        return NULL;
    return &matrix_memory_table[index];
}

void compute_memory_layout(MatrixIR *ir, MemoryLayout *layout) {
    // Compute dynamic addresses based on IR dimensions.
    layout->base_A = 0x0000;
    layout->base_B = layout->base_A + (ir->rows_A * ir->cols_A);
    layout->base_V = layout->base_B;
    layout->base_C = layout->base_B + (ir->rows_B * ir->cols_B);
}
