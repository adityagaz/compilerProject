#include <stdio.h>
#include "ir_optimizer.h"

OptimizedInfo optimize_ir(MatrixIR *ir) {
    OptimizedInfo opt;
    // Calculate the number of Multiply-Accumulate (MAC) steps.
    // For multiplication: MAC steps = rows_A * cols_B * cols_A
    opt.mac_steps = ir->rows_A * ir->cols_B * ir->cols_A;
    // For simplicity, set partial multiplications equal to MAC steps.
    opt.partial_mults = opt.mac_steps;
    return opt;
}
