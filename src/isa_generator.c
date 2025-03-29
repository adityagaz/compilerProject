#include <stdio.h>
#include <stdlib.h>
#include "isa_generator.h"
#include "isa_encoding.h"
#include "target_memory.h"

// Generates a READ instruction using the computed memory layout.
static void generate_read_instruction(char operand, int index1, int index2, MatrixIR *ir, MemoryLayout *layout, FILE *fp) {
    unsigned int address = 0;
    if (operand == 'A') {
        // For matrix A: address = base_A + (i * cols_A + k)
        address = layout->base_A + (index1 * ir->cols_A + index2);
    } else if (operand == 'B') {
        // For matrix B (matrix-matrix): address = base_B + (k * cols_B + j)
        address = layout->base_B + (index1 * ir->cols_B + index2);
    } else if (operand == 'V') {
        // For vector (matrix-vector): address = base_V + index1
        address = layout->base_V + index1;
    }
    int opcode_type = 0; // READ opcode.
    int pointer = 0;
    int read_flag = 1;
    int write_flag = 0;
    unsigned int instruction = encode_instruction(opcode_type, pointer, read_flag, write_flag, address);
    if (operand == 'A' || operand == 'B')
        fprintf(fp, "  READ %c[%d][%d]: 0x%06X\n", operand, index1, index2, instruction);
    else  // For vector, index2 is not used.
        fprintf(fp, "  READ %c[%d]: 0x%06X\n", operand, index1, instruction);
}

// Generates control instructions (PROG, EXE, END) for a result element.
static void generate_control_instructions(int i, int j, MatrixIR *ir, MemoryLayout *layout, FILE *fp) {
    unsigned int address = 0;
    if (ir->cols_B > 1) {
        // Matrix-matrix multiplication: C[i][j] address.
        address = layout->base_C + (i * ir->cols_B + j);
    } else {
        // Matrix-vector multiplication: C[i] address.
        address = layout->base_C + i;
    }
    int pointer = 0;
    int read_flag = 0;
    int write_flag = 0;
    
    unsigned int prog = encode_instruction(1, pointer, read_flag, write_flag, address);
    unsigned int exe  = encode_instruction(2, pointer, read_flag, write_flag, address);
    unsigned int end  = encode_instruction(3, pointer, read_flag, write_flag, address);
    
    fprintf(fp, "  PROG: 0x%06X\n", prog);
    fprintf(fp, "  EXE:  0x%06X\n", exe);
    fprintf(fp, "  END:  0x%06X\n", end);
}

void generate_instruction_sequence(MatrixIR *ir, OptimizedInfo *opt) {
    FILE *fp = fopen("output.isa", "w");
    if (!fp) {
        perror("Failed to open output.isa");
        exit(EXIT_FAILURE);
    }
    
    // Write a pretty header.
    fprintf(fp, "============================================================\n");
    fprintf(fp, "    Generated Instruction Sequence (24-bit ISA Format)    \n");
    fprintf(fp, "============================================================\n\n");
    
    // Initialize a counter for the total number of instructions generated.
    int total_instructions = 0;
    
    // Compute memory layout based on IR.
    MemoryLayout layout;
    compute_memory_layout(ir, &layout);
    
    // Generate instructions based on multiplication type.
    if (ir->cols_B > 1) {
        // Matrix-Matrix Multiplication.
        int m = ir->rows_A;
        int n = ir->cols_A;  // Also equals ir->rows_B.
        int p = ir->cols_B;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < p; j++) {
                fprintf(fp, "Processing C[%d][%d]\n", i, j);
                for (int k = 0; k < n; k++) {
                    // Read A[i][k]: computed from base_A.
                    generate_read_instruction('A', i, k, ir, &layout, fp);
                    total_instructions++;
                    // Read B[k][j]: computed from base_B.
                    generate_read_instruction('B', k, j, ir, &layout, fp);
                    total_instructions++;
                }
                // Control instructions for C[i][j]: PROG, EXE, and END.
                generate_control_instructions(i, j, ir, &layout, fp);
                total_instructions += 3;  // Three control instructions.
                fprintf(fp, "\n");
            }
        }
    } else {
        // Matrix-Vector Multiplication.
        int m = ir->rows_A;
        int n = ir->cols_A;
        for (int i = 0; i < m; i++) {
            fprintf(fp, "Processing C[%d]\n", i);
            for (int k = 0; k < n; k++) {
                generate_read_instruction('A', i, k, ir, &layout, fp);
                total_instructions++;
                // For matrix-vector, use operand 'V' for vector.
                generate_read_instruction('V', k, 0, ir, &layout, fp);
                total_instructions++;
            }
            generate_control_instructions(i, 0, ir, &layout, fp);
            total_instructions += 3; // Three control instructions.
            fprintf(fp, "\n");
        }
    }

    int total_bits = total_instructions * 24;

    fprintf(fp, "============================================================\n");
    fprintf(fp, "Total instructions generated: %d\n", total_instructions);
    fprintf(fp, "Total bits: %d\n", total_bits);
    fprintf(fp, "============================================================\n");
    
    fclose(fp);
    printf("Instruction sequence generated in output.isa\n");
}
