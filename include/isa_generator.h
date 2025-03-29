#ifndef ISA_GENERATOR_H
#define ISA_GENERATOR_H

#include "semantic_analyzer.h"
#include "ir_optimizer.h"

// Generates the ISA instruction sequence.
void generate_instruction_sequence(MatrixIR *ir, OptimizedInfo *opt);

#endif // ISA_GENERATOR_H
