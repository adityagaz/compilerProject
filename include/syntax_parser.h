#ifndef SYNTAX_PARSER_H
#define SYNTAX_PARSER_H

// Supported operation types.
typedef enum {
    MM_MULT,   // Matrix-Matrix multiplication
    MV_MULT    // Matrix-Vector multiplication
} OperationType;

// Structure to hold parsed information.
typedef struct {
    OperationType opType;
    int m;  // For matrix A rows (or result vector rows in MV)
    int n;  // For matrix A columns (and vector length in MV)
    int p;  // For matrix B columns (for MM_MULT; for MV_MULT, p = 1)
} ParsedInfo;

// Parses the source code and extracts the operation type and dimensions.
// Expected markers in the file are:
//   // OPERATION: MM_MULT or MV_MULT
//   // DIMENSIONS: m=3 n=4 p=5   (or for MV_MULT, p can be omitted)
ParsedInfo parse_source(const char* source_code);

#endif // SYNTAX_PARSER_H
