#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu
{
  // TODO
  // PC
  unsigned int pc;
  // registers (array)
  unsigned char *registers;
  // ram (array)
  unsigned char *ram;
  //Flag registers array
  unsigned char flags;
};

// ALU operations
enum alu_op
{
  ALU_MUL,
  ALU_DIV,
  ALU_ADD,
  ALU_SUB,
  ALU_CMP,
  ALU_AND,
  ALU_OR
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010
#define PRN 0b01000111
#define HLT 0b00000001
#define MUL 0b10100010
#define DIV 0b10100011
#define ADD 0b10100000
#define SUB 0b10100001
#define PUSH 0b01000101
#define POP 0b01000110
#define CALL 0b01010000
#define RET 0b00010001
#define CMP 0b10100111
#define JEQ 0b01010101
#define JGE 0b01011010
#define JGT 0b01010111
#define JLE 0b01011001
#define JLT 0b01011000
#define JMP 0b01010100
#define JNE 0b01010110
#define AND 0b10101000
#define OR 0b10101010
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *file_name);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);
extern int cpu_ram_read(struct cpu *cpu, unsigned char index);
extern void cpu_ram_write(struct cpu *cpu, unsigned char index, int value);

#endif
