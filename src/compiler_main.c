#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntax_parser.h"
#include "semantic_analyzer.h"
#include "ir_optimizer.h"
#include "isa_generator.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source_file.cpp>\n", argv[0]);
        return 1;
    }
    
    // Open the input C++ source file.
    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Error opening source file");
        return 1;
    }
    
    // Determine the file size.
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);
    
    // Allocate buffer and read the file contents.
    char *source_code = malloc(file_size + 1);
    if (!source_code) {
        fprintf(stderr, "Memory allocation error\n");
        fclose(fp);
        return 1;
    }
    fread(source_code, 1, file_size, fp);
    source_code[file_size] = '\0';
    fclose(fp);
    
    printf("Loaded Source Code:\n%s\n", source_code);
    
    // Parse the source code to extract operation type and dimensions.
    ParsedInfo parsed = parse_source(source_code);
    printf("Parsed Info: Operation Type: %s, m=%d, n=%d, p=%d\n", 
           (parsed.opType == MM_MULT) ? "Matrix-Matrix" : "Matrix-Vector",
           parsed.m, parsed.n, parsed.p);
    
    // Perform semantic analysis to build the intermediate representation.
    MatrixIR ir = perform_semantic_analysis(parsed);
    printf("IR: Matrix A: %dx%d, Matrix B: %dx%d\n", 
            ir.rows_A, ir.cols_A, ir.rows_B, ir.cols_B);
    
    // Optimize the IR.
    OptimizedInfo opt = optimize_ir(&ir);
    printf("Optimized Info: MAC steps=%d, partial mults=%d\n", 
           opt.mac_steps, opt.partial_mults);
    
    // Generate the ISA instruction sequence (this writes output.isa).
    generate_instruction_sequence(&ir, &opt);
    
    free(source_code);
    return 0;
}
