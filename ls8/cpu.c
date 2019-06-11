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
  int operandA = cpu_ram_read(cpu, regA);
  int operandB = cpu_ram_read(cpu, regB);
  float result;

  switch (op)
  {
  case ALU_MUL:
    result = (operandA * operandB);
    break;

  case ALU_DIV:
    result = (operandA / operandB);
    break;

  case ALU_ADD:
    result = (operandA + operandB);
    break;

  case ALU_SUB:
    result = (operandA - operandB);
    break;
  }
  printf("Result: %f\n", result);
}

/**
 * Run the CPU 
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char ir;
  unsigned int v, regA, regB;

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
      regA = cpu->ram[cpu->pc + 1];
      v = cpu_ram_read(cpu, regA);
      printf("%d\n", v);
      break;

    case LDI:
      regA = cpu->ram[cpu->pc + 1];
      v = cpu->ram[cpu->pc + 2];
      printf("Saving %u\n", v);
      cpu_ram_write(cpu, regA, v);
      break;

    case MUL:
      regA = cpu->ram[cpu->pc + 1];
      regB = cpu->ram[cpu->pc + 2];
      alu(cpu, ALU_MUL, regA, regB);
      break;

    case DIV:
      regA = cpu->ram[cpu->pc + 1];
      regB = cpu->ram[cpu->pc + 2];
      alu(cpu, ALU_DIV, regA, regB);
      break;

    case ADD:
      regA = cpu->ram[cpu->pc + 1];
      regB = cpu->ram[cpu->pc + 2];
      alu(cpu, ALU_ADD, regA, regB);
      break;

    case SUB:
      regA = cpu->ram[cpu->pc + 1];
      regB = cpu->ram[cpu->pc + 2];
      alu(cpu, ALU_SUB, regA, regB);
      break;

    default:
      printf("Unknown instruction %02x at address %02x\n", ir, cpu->pc);
      exit(1);
    }
    int number_of_operands = ((ir >> 6) & 0b11) + 1;
    cpu->pc += number_of_operands;
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

int cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->registers[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, int value)
{
  cpu->registers[index] = value;
}
