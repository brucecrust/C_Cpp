#include <stdio.h>
#include <stdbool.h>

// Definitions and Arrays:

typedef enum {
    PSH,    // Push a given value to the stack.
    ADD,    // Grab the last two values from the stack and push the sum back to the stack.
    POP,    // Pops the last value off the stack and prints it.
    SET,    // Sets register A to 0.
    HLT     // Halts the program.
} InstructionSet;

typedef enum {
    A, B, C, D, E, F, PC, SP,
    REGISTER_SIZE
} Registers;

const int instructions[] = {
        PSH, 5,
        PSH, 6,
        ADD,
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
            int valPopped = stack[registers[SP]];
            registers[SP] -= 1;
            printf("Popped: %d\n", valPopped);
            break;
        }

        case ADD: {
            int a = stack[registers[SP]];
            registers[SP] -= 1;
            int b = stack[registers[SP]];
            registers[SP] -= 1;
            int result = a + b;

            registers[SP] += 1;
            stack[registers[SP]] = result;

            printf("Sum: %d\n", result);

            break;
        }

        case SET: {
            registers[instructions[PC + 1]] = registers[instructions[PC + 2]];
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
