#include <stdio.h>
#include <stdbool.h>

int pc = 0;
bool running = true;

typedef enum {
    PSH,
    ADD,
    POP,
    SET,
    HLT
} InstructionSet;

const int program[] = {
        PSH, 5,
        PSH, 6,
        ADD,
        POP,
        HLT
};

void evaluateInstruction(int instruction) {
    switch (instruction) {
        case HLT:
            running = true;
            break;

        default:
            break;
    }
}

int fetch() {
    return program[pc];
}

int main() {
    while (running) {
        evaluateInstruction(fetch());
        ++pc;
    }
    return 0;
}
