#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
      // From print8.ls8
      0b10000010, // LDI R0,8
      0b00000000,
      0b00001000,
      0b01000111, // PRN R0
      0b00000000,
      0b00000001 // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++)
  {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    // TODO
    break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU 
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char ir;
  unsigned int v, reg;

  for (int i = 0; i < 6; i++)
  {
    // printf("Command: %d, ");
  }

  while (running)
  {
    ir = cpu->ram[cpu->pc];
    switch (ir)
    {
    case HLT:
      printf("Exiting!\n");
      running = 0;
      exit(1);
      break;

    case PRN:
      printf("Printing!\n");
      reg = cpu->ram[cpu->pc++];
      v = cpu_ram_read(cpu, reg);
      printf("%u\n", v);
      cpu->pc++;
      break;

    case LDI:
      printf("Saving!\n");
      v = cpu->ram[cpu->pc++];
      reg = cpu->ram[cpu->pc++];
      cpu->registers[reg] = v;
      cpu->pc++;
      break;

    default:
      printf("Unknown instruction %02x at address %02x\n", ir, cpu->pc);
      cpu->pc++;
      // exit(1);
    }
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->pc = 0;
  cpu->registers = malloc(8 * sizeof(unsigned char));
  cpu->ram = malloc(128 * sizeof(unsigned char));
}

unsigned int cpu_ram_read(struct cpu *cpu, unsigned int index) {
  return cpu->registers[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned int value, unsigned int index) {
  cpu->registers[index] = value;
}

