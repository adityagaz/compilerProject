# Matrix Multiplication Compiler for Custom 24-bit ISA

## Overview
This compiler translates matrix multiplication (both matrix–matrix and matrix–vector multiplication) defined in C++ source files into a sequence of instructions for a custom-designed 24-bit Instruction Set Architecture (ISA). The compiler pipeline includes parsing, semantic analysis, intermediate representation (IR) optimization, and ISA code generation.

---

## Features
- **Supported Operations**
  - Matrix–Matrix Multiplication (`MM_MULT`)
  - Matrix–Vector Multiplication (`MV_MULT`)

- **Input Format**  
  Reads C++ source files with special markers:
  ```cpp
  // OPERATION: MM_MULT
  // DIMENSIONS: m=3 n=4 p=5
  ```

- **24-bit ISA Encoding**
  | Bits  | Field         | Description                      |
  |-------|---------------|----------------------------------|
  | 23-22 | Opcode Type   | READ(0), PROG(1), EXE(2), END(3) |
  | 21-16 | Pointer       | Control or addressing pointer    |
  | 15    | Read Flag     | Indicates memory read operation  |
  | 14    | Write Flag    | Indicates memory write operation |
  | 13-0  | Address       | Operand memory address           |

- **Memory Layout**
  Automatically computes base addresses:
  - Matrix A starts at `0x0000`
  - Matrix B (or Vector V) follows immediately after A
  - Matrix C follows immediately after B/V

- **Formatted Output**  
  Generates a clear, readable `output.isa` instruction file.

- **Lookup Tables**
  Includes a utility executable (`print_tables`) to display ISA opcodes and memory base addresses.

---

## Directory Structure
```
MatrixMultiplicationCompiler/
├── Makefile
├── README.md
├── include/
│   ├── isa_encoding.h
│   ├── isa_generator.h
│   ├── ir_optimizer.h
│   ├── semantic_analyzer.h
│   ├── syntax_parser.h
│   └── target_memory.h
└── src/
    ├── compiler_main.c
    ├── isa_encoding.c
    ├── isa_generator.c
    ├── ir_optimizer.c
    ├── print_tables.c
    ├── semantic_analyzer.c
    ├── syntax_parser.c
    └── target_memory.c
```

---

## Setup & Compilation

### Prerequisites
- GCC (C99 support required)
- Make
- OpenMP (included by default with modern GCC)

Install prerequisites (Ubuntu/Debian):
```bash
sudo apt update
sudo apt install build-essential
```

### Building the Project
Run the following commands in the project root directory:
```bash
make clean
make
```

This generates:
- `matrix_compiler` (the main executable)
- `print_tables` (the utility executable)

---

## Usage

### Step 1: Prepare Input File
Create a C++ source file (e.g., `example.cpp`) with operation markers:
```cpp
// OPERATION: MM_MULT
// DIMENSIONS: m=3 n=4 p=5

#include <iostream>
using namespace std;

void matmul(int A[3][4], int B[4][5], int C[3][5]) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 5; j++) {
            C[i][j] = 0;
            for (int k = 0; k < 4; k++)
                C[i][j] += A[i][k] * B[k][j];
        }
}

int main() {
    int A[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    int B[4][5] = {
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 10},
        {11, 12, 13, 14, 15},
        {16, 17, 18, 19, 20}
    };

    int C[3][5] = {0};

    matmul(A, B, C);

    cout << "Result Matrix C:" << endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++)
            cout << C[i][j] << " ";
        cout << endl;
    }
    return 0;
}
```

### Step 2: Run the Compiler
Execute the following command:
```bash
./matrix_compiler example.cpp
```
This will produce `output.isa`.

---

## Viewing the ISA and Memory Tables
To view ISA opcodes and matrix memory base addresses:
```bash
./print_tables
```

Example Output:
```
=== Instruction Opcode Table ===
Index  Instruction Name     Opcode
-----  -------------------  ------
0      READ                 0x0
1      PROG                 0x1
2      EXE                  0x2
3      END                  0x3

=== Matrix Base Address Table ===
Index  Identifier   Base Address
-----  ----------   ------------
0      A            0x0000
1      B            0x000C
2      C            0x0020
3      V            0x000C
```

---

## Sample Generated Output (`output.isa`)
```
=====================================================
  Generated Instruction Sequence (24-bit ISA Format)
=====================================================

Processing C[0][0]
  READ A[0][0]: 0x008000
  READ B[0][0]: 0x00800C
  READ A[0][1]: 0x008001
  READ B[1][0]: 0x008011
  READ A[0][2]: 0x008002
  READ B[2][0]: 0x008016
  READ A[0][3]: 0x008003
  READ B[3][0]: 0x00801B
  PROG: 0x400020
  EXE:  0x800020
  END:  0xC00020

...

=====================================================
Total instructions generated: 120
Total bits: 2880
=====================================================
```

---

## Contributing
Contributions are welcome! Feel free to fork, open issues, or submit pull requests.

---

## License
Distributed under the MIT License.
