#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "syntax_parser.h"

// Intermediate representation for matrices.
typedef struct {
    int rows_A;
    int cols_A;
    int rows_B;
    int cols_B;
} MatrixIR;

// Analyzes the parsed info and builds the IR.
MatrixIR perform_semantic_analysis(ParsedInfo info);

#endif // SEMANTIC_ANALYZER_H
