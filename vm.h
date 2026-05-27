#ifndef VM_H
#define VM_H

#include "compiler.h"

#define STACK_SIZE 256
#define MAX_VARIABLES 256

typedef struct {
    double stack[STACK_SIZE];

    int sp;

    int ip;

    double variables[MAX_VARIABLES];

} VM;

void vm_run(Bytecode* bc);

#endif