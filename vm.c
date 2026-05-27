#define _USE_MATH_DEFINES

#include <stdio.h>
#include <math.h>

#include "vm.h"

static void push(VM* vm, double value)
{
    vm->stack[vm->sp++] = value;
}

static double pop(VM* vm)
{
    return vm->stack[--vm->sp];
}

void vm_run(Bytecode* bc)
{
    VM vm;

    vm.sp = 0;
    vm.ip = 0;

    while (1)
    {
        int instruction = bc->code[vm.ip++];

        switch (instruction)
        {
            case OP_PUSH:
            {
                int index = bc->code[vm.ip++];

                push(&vm, bc->constants[index]);

                break;
            }

            case OP_ADD:
            {
                double b = pop(&vm);
                double a = pop(&vm);

                push(&vm, a + b);

                break;
            }

            case OP_SUB:
            {
                double b = pop(&vm);
                double a = pop(&vm);

                push(&vm, a - b);

                break;
            }

            case OP_MUL:
            {
                double b = pop(&vm);
                double a = pop(&vm);

                push(&vm, a * b);

                break;
            }

            case OP_DIV:
            {
                double b = pop(&vm);
                double a = pop(&vm);

                push(&vm, a / b);

                break;
            }

            case OP_STORE:
            {
                int slot = bc->code[vm.ip++];

                vm.variables[slot] = pop(&vm);

                break;
            }

            case OP_LOAD:
            {
                int slot = bc->code[vm.ip++];

                push(&vm, vm.variables[slot]);

                break;
            }

            case OP_PRINT:
            {
                printf("%lf\n", pop(&vm));

                break;
            }

            case OP_SIN:
            {
                double value = pop(&vm);

                value = value * M_PI / 180.0;

                push(&vm, sin(value));

                break;
            }

            case OP_COS:
            {
                double value = pop(&vm);

                value = value * M_PI / 180.0;

                push(&vm, cos(value));

                break;
            }

            case OP_TAN:
            {
                double value = pop(&vm);

                value = value * M_PI / 180.0;

                push(&vm, tan(value));

                break;
            }

            case OP_SQRT:
            {
                double value = pop(&vm);

                push(&vm, sqrt(value));

                break;
            }

            case OP_POW:
            {
                double exponent = pop(&vm);

                double base = pop(&vm);

                push(&vm, pow(base, exponent));

                break;
            }

            case OP_HALT:
            {
                return;
            }

            default:
            {
                printf("Unknown opcode: %d\n", instruction);

                return;
            }
        }
    }
}