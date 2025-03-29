#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntax_parser.h"

ParsedInfo parse_source(const char* source_code) {
    ParsedInfo info;
    // Set default values.
    info.opType = MM_MULT;  // default to matrix-matrix
    info.m = 0;
    info.n = 0;
    info.p = 0;
    
    // Look for the "OPERATION:" marker.
    const char *op_marker = strstr(source_code, "OPERATION:");
    if (op_marker) {
        op_marker += strlen("OPERATION:");
        while (*op_marker == ' ' || *op_marker == '\t') op_marker++;
        if (strncmp(op_marker, "MV_MULT", 7) == 0)
            info.opType = MV_MULT;
        else
            info.opType = MM_MULT;
    }
    
    // Look for the "DIMENSIONS:" marker.
    const char *dim_marker = strstr(source_code, "DIMENSIONS:");
    if (dim_marker) {
        dim_marker += strlen("DIMENSIONS:");
        char dims[128];
        sscanf(dim_marker, "%127[^\n]", dims); // read until newline
        
        // Tokenize the dimensions string.
        char *token = strtok(dims, " ");
        while (token != NULL) {
            if (strncmp(token, "m=", 2) == 0)
                info.m = atoi(token + 2);
            else if (strncmp(token, "n=", 2) == 0)
                info.n = atoi(token + 2);
            else if (strncmp(token, "p=", 2) == 0)
                info.p = atoi(token + 2);
            token = strtok(NULL, " ");
        }
    }
    
    // For matrix-vector multiplication, set p to 1 if not provided.
    if (info.opType == MV_MULT)
        info.p = 1;
    
    // Basic validation.
    if (info.m <= 0 || info.n <= 0 || info.p <= 0) {
        fprintf(stderr, "Invalid or missing dimensions in source code.\n");
        exit(EXIT_FAILURE);
    }
    
    return info;
}
