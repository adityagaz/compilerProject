#include <stdio.h>
#include "isa_encoding.h"     // For InstructionInfo and lookup functions.
#include "target_memory.h"    // For MatrixMemoryInfo and lookup functions.

int main() {
    printf("========================================\n");
    printf("         Detailed Lookup Tables         \n");
    printf("========================================\n\n");

    // Instruction Opcode Table
    printf("=== Instruction Opcode Table ===\n");
    printf("%-5s %-20s %-10s\n", "Index", "Instruction Name", "Opcode");
    printf("----- -------------------- ----------\n");
    int instr_count = get_instruction_table_size();
    for (int i = 0; i < instr_count; ++i) {
        const InstructionInfo *info = get_instruction_info_by_index(i);
        if (info != NULL) {
            printf("%-5d %-20s 0x%X\n", i, info->name, info->opcode);
        }
    }
    printf("\nTotal Instructions: %d\n\n", instr_count);

    // Matrix Base Address Table
    printf("=== Matrix Base Address Table ===\n");
    printf("%-5s %-15s %-10s\n", "Index", "Identifier", "Base Address");
    printf("----- --------------- ----------\n");
    int matrix_count = get_matrix_memory_table_size();
    for (int i = 0; i < matrix_count; ++i) {
        const MatrixMemoryInfo *info = get_matrix_memory_info_by_index(i);
        if (info != NULL) {
            printf("%-5d %-15c 0x%X\n", i, info->identifier, info->base_address);
        }
    }
    printf("\nTotal Matrix Memory Entries: %d\n", matrix_count);

    printf("\n========================================\n");
    return 0;
}
