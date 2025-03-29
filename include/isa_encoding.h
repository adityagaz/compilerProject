#ifndef ISA_ENCODING_H
#define ISA_ENCODING_H

// Structure for an instruction's lookup information.
typedef struct {
    const char *name;
    unsigned int opcode;
} InstructionInfo;

// Encodes a 24-bit instruction using the following bit layout:
// Bits 23-22: opcode type, Bits 21-16: pointer, Bit 15: read flag,
// Bit 14: write flag, Bits 13-0: address.
unsigned int encode_instruction(int opcode_type, int pointer, int read_flag, int write_flag, int address);

// Lookup table accessor functions.
int get_instruction_table_size(void);
const InstructionInfo * get_instruction_info_by_index(int index);

#endif // ISA_ENCODING_H
