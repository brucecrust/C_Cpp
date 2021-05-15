#include <stdio.h>
#include <stdbool.h>

// Definitions and Arrays:

typedef enum {
    PSH,
    ADD,
    SUB,
    MUL,
    DIV,
    LOG,
    POP,
    SET,
    HLT
} InstructionSet;

typedef enum {
    A, B, C, D, E, F, PC, SP,
    REGISTER_SIZE
} Registers;

// Demonstrate: 5+6=11, 10-5=5, 3*3=9, 36/6=6.
const int instructions[] = {
        PSH, 5,
        PSH, 6,
        ADD,
        LOG,
        POP,
        PSH, 10,
        PSH, 5,
        SUB,
        LOG,
        POP,
        PSH, 3,
        PSH, 3,
        MUL,
        LOG,
        POP,
        PSH, 36,
        PSH, 6,
        DIV,
        LOG,
        POP,
        HLT
};

// Variables:

int stack[256];
int registers[REGISTER_SIZE];
bool running = true;

// Functions:

void evaluateInstruction(int instruction) {
    switch (instruction) {
        case HLT: {
            running = false;
            break;
        }

        case PSH: {
            registers[SP] += 1;
            registers[PC] += 1;

            stack[registers[SP]] = instructions[registers[PC]];

            break;
        }

        case POP: {
            registers[SP] -= 1;

            break;
        }

        case ADD: {
            int a = stack[registers[SP]];

            registers[SP] -= 1;

            int b = stack[registers[SP]];
            int result = a + b;

            stack[registers[SP]] = result;

            break;
        }

        case SUB: {
            registers[SP] -= 1;

            int a = stack[registers[SP]];

            registers[SP] += 1;

            int b = stack[registers[SP]];

            int result = a - b;

            registers[SP] -= 1;

            stack[registers[SP]] = result;

            break;
        }

        case MUL: {
            int a = stack[registers[SP]];

            registers[SP] -= 1;

            int b = stack[registers[SP]];
            int result = a * b;

            stack[registers[SP]] = result;

            break;
        }

        case DIV: {
            registers[SP] -= 1;

            int a = stack[registers[SP]];

            registers[SP] += 1;

            int b = stack[registers[SP]];

            int result = 0;

            registers[SP] -= 1;

            if (a != 0 && b != 0) {
                result = a / b;
            }

            stack[registers[SP]] = result;

            break;
        }

        case SET: {
            registers[instructions[PC + 1]] = registers[instructions[PC + 2]];
            registers[PC] += 2;

            break;
        }

        case LOG: {
            printf("Log: %d\n", stack[registers[SP]], registers[SP]);

            break;
        }

        default:
            break;
    }
}

int fetch() {
    return instructions[registers[PC]];
}

int main() {
    registers[PC] = 0;
    registers[SP] = -1;
    while (running) {
        evaluateInstruction(fetch());
        registers[PC] += 1;
    }

    return 0;
}
