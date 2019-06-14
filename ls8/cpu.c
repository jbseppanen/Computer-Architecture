#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

#define DATA_LEN 6
#define SP 7
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file_name)
{
  FILE *fp;
  char line[1024];
  fp = fopen(file_name, "r");

  if (fp == NULL)
  {
    fprintf(stderr, "file not found\n");
    exit(1);
  }

  int mar = 0;
  while (fgets(line, 1024, fp) != NULL)
  {
    char *endptr;
    unsigned char v = strtoul(line, &endptr, 2);
    if (!(endptr == line)) //Check for empty/bad lines
    {
      cpu->ram[mar] = v;
      mar++;
    }
  }
  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  int operandA = cpu->registers[regA];
  int operandB = cpu->registers[regB];
  int result;

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

  case ALU_CMP:
    if (operandA == operandB)
    {
      cpu->flags = cpu->flags | 0b00000001;
      cpu->flags = cpu->flags & 0b11111001;
    } else if (operandA > operandB)
    {
      cpu->flags = cpu->flags | 0b00000010;
      cpu->flags = cpu->flags & 0b11111010;
    } else {
      cpu->flags = cpu->flags | 0b00000100;
      cpu->flags = cpu->flags & 0b11111100;
    }
    printf("Flags: "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(cpu->flags));
    result = cpu->flags;
    break;
  }
  cpu->registers[regA] = result;
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
      running = 0;
      exit(0);
      break;

    case PRN:
      regA = cpu_ram_read(cpu, cpu->pc + 1);
      v = cpu->registers[regA];
      printf("%d\n", v);
      break;

    case LDI:
      regA = cpu_ram_read(cpu, cpu->pc + 1);
      v = cpu->ram[cpu->pc + 2];
      cpu->registers[regA] = v;
      break;

    case MUL:
      regA = cpu_ram_read(cpu, cpu->pc + 1);
      regB = cpu_ram_read(cpu, cpu->pc + 2);
      alu(cpu, ALU_MUL, regA, regB);
      break;

    case DIV:
      regA = cpu_ram_read(cpu, cpu->pc + 1);
      regB = cpu_ram_read(cpu, cpu->pc + 2);
      alu(cpu, ALU_DIV, regA, regB);
      break;

    case ADD:
      regA = cpu_ram_read(cpu, cpu->pc + 1);
      regB = cpu_ram_read(cpu, cpu->pc + 2);
      alu(cpu, ALU_ADD, regA, regB);
      break;

    case SUB:
      regA = cpu_ram_read(cpu, cpu->pc + 1);
      regB = cpu_ram_read(cpu, cpu->pc + 2);
      alu(cpu, ALU_SUB, regA, regB);
      break;

    case PUSH:
      cpu->registers[SP]--;
      regA = cpu_ram_read(cpu, cpu->pc + 1);
      v = cpu->registers[regA];
      cpu_ram_write(cpu, cpu->registers[SP], v);
      break;

    case POP:
      regA = cpu_ram_read(cpu, cpu->pc + 1);
      cpu->registers[regA] = cpu_ram_read(cpu, cpu->registers[SP]);
      cpu->registers[SP]++;
      break;

    case CALL:
      cpu->registers[SP]--;
      v = cpu->pc + 2;
      cpu_ram_write(cpu, cpu->registers[SP], v);
      regA = cpu_ram_read(cpu, cpu->pc + 1);
      cpu->pc = cpu->registers[regA];
      break;

    case RET:
      cpu->pc = cpu_ram_read(cpu, cpu->registers[SP]);
      cpu->registers[SP]++;
      break;

    case CMP:
      regA = cpu_ram_read(cpu, cpu->pc + 1);
      regB = cpu_ram_read(cpu, cpu->pc + 2);
      alu(cpu, ALU_CMP, regA, regB);
      break;

    default:
      printf("Unknown instruction %02x at address %02x\n", ir, cpu->pc);
      exit(1);
    }

    int set_pc = ((ir >> 4) & 0b0001);
    if (!set_pc)
    {
      int pc_increment = ((ir >> 6) & 0b11) + 1;
      cpu->pc += pc_increment;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->pc = 0;
  cpu->registers = malloc(8 * sizeof(unsigned char));
  memset(cpu->registers, 0, 8 * sizeof(unsigned char));
  cpu->ram = malloc(128 * sizeof(unsigned char));
  memset(cpu->ram, 0, 128 * sizeof(unsigned char));
  cpu->registers[SP] = 0xF4; //Set pointer to default.
  cpu->flags = 0b00000000;
}

int cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, int value)
{
  cpu->ram[index] = value;
}
