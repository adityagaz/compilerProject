#ifndef IR_OPTIMIZER_H
#define IR_OPTIMIZER_H

#include "semantic_analyzer.h"

// Optimized information structure.
typedef struct {
    int mac_steps;
    int partial_mults;
} OptimizedInfo;

// Optimizes the IR (currently a simple calculation).
OptimizedInfo optimize_ir(MatrixIR *ir);

#endif // IR_OPTIMIZER_H
