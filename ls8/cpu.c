#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file_name)
{
  FILE *fp;
  char line[1024];
  printf("File Name:%s\n", file_name);
  fp = fopen(file_name, "r");

  if (fp == NULL)
  {
    fprintf(stderr, "file not found\n");
    exit(1);
  }

  int address = 0;
  while (fgets(line, 1024, fp) != NULL)
  {
    char *endptr;
    unsigned char v = strtoul(line, &endptr, 2);
    if (!(endptr == line)) //Check for empty/bad lines
    {
      cpu->ram[address] = v;
      address++;
    }
  }
  fclose(fp);
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
      exit(0);
      break;

    case PRN:
      printf("Printing!\n");
      reg = cpu->ram[cpu->pc + 1];
      v = cpu_ram_read(cpu, reg);
      printf("%d\n", v);
      cpu->pc += 2;
      break;

    case LDI:
      reg = cpu->ram[cpu->pc + 1];
      v = cpu->ram[cpu->pc + 2];
      printf("Saving %u\n", v);
      cpu_ram_write(cpu, reg, v);
      unsigned int temp = cpu_ram_read(cpu, reg);
      cpu->pc += 3;
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

int cpu_ram_read(struct cpu *cpu, unsigned int index)
{
  return cpu->registers[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned int index, unsigned int value)
{
  cpu->registers[index] = value;
}
