#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char **argv)
{
  struct cpu cpu;
  cpu_init(&cpu);
  for (int i = 1; i < argc; i++)
  {
  // cpu_load(&cpu, "../ls8/examples/print8.ls8");
  cpu_load(&cpu, argv[i]);
  cpu_run(&cpu);
  }
  return 0;
}