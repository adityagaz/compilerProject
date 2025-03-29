#include <stdio.h>
#include <stdlib.h>
#include "semantic_analyzer.h"

MatrixIR perform_semantic_analysis(ParsedInfo info) {
    MatrixIR ir;
    
    if (info.opType == MM_MULT) {
        // For matrix-matrix multiplication, assume:
        // A is m x n, and B is n x p.
        ir.rows_A = info.m;
        ir.cols_A = info.n;
        ir.rows_B = info.n;
        ir.cols_B = info.p;
    } else {
        // For matrix-vector multiplication, assume:
        // A is m x n and vector V is n x 1.
        ir.rows_A = info.m;
        ir.cols_A = info.n;
        ir.rows_B = info.n;
        ir.cols_B = 1;
    }
    
    // Validate that the inner dimensions match.
    if (ir.cols_A != ir.rows_B) {
        fprintf(stderr, "Dimension mismatch: A's columns (%d) must equal B's rows (%d).\n", ir.cols_A, ir.rows_B);
        exit(EXIT_FAILURE);
    }
    
    return ir;
}
