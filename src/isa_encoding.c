#include "isa_encoding.h"
#include <stdio.h>
// Function to encode a 24-bit instruction.
unsigned int encode_instruction(int opcode_type, int pointer, int read_flag, int write_flag, int address) {
    unsigned int instruction = 0;
    instruction |= ((opcode_type & 0x3) << 22);    // Bits 23-22.
    instruction |= ((pointer & 0x3F) << 16);         // Bits 21-16.
    instruction |= ((read_flag & 0x1) << 15);          // Bit 15.
    instruction |= ((write_flag & 0x1) << 14);         // Bit 14.
    instruction |= (address & 0x3FFF);                 // Bits 13-0.
    return instruction;
}

// Define a static lookup table for instruction opcodes.
static InstructionInfo instruction_table[] = {
    {"READ", 0x00},   // Opcode for READ instruction.
    {"PROG", 0x01},   // Opcode for PROG (start) instruction.
    {"EXE",  0x02},   // Opcode for EXE (execute) instruction.
    {"END",  0x03}    // Opcode for END instruction.
};

// Returns the number of instructions in the lookup table.
int get_instruction_table_size(void) {
    return sizeof(instruction_table) / sizeof(instruction_table[0]);
}

// Returns a pointer to the InstructionInfo for the given index.
const InstructionInfo * get_instruction_info_by_index(int index) {
    if (index < 0 || index >= get_instruction_table_size())
        return NULL;
    return &instruction_table[index];
}
