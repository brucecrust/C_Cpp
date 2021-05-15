#include "Chip8.h"

// Opcode Methods:

/**
 * Clear the display. Using memset, set the video buffer to the value of 0.
 * */
void Chip8::OP_00E0() {
    int value = 0;
    memset(mVideo, value, sizeof(mVideo));
}

/**
 * Return from a given subroutine. Top of the stack holds the address of one instruction past the one that called
 * the given subroutine. Put that address back into the program counter and decrement the stack pointer.
 * */
void Chip8::OP_00EE() {
    --mSP;
    mPC = mStack[mSP];
}

/**
 * Set the program counter to nnn. Jump doesn't remember its origin, so no need to alter the stack.
 * */
void Chip8::OP_1nnn() {
    uint16_t address = mOpcode & 0x0FFFu;
    mPC = address;
}

void Chip8::OP_2nnn() {

}

void Chip8::OP_3xkk() {}

void Chip8::OP_4xkk() {}

void Chip8::OP_5xy0() {}

void Chip8::OP_6xkk() {}

void Chip8::OP_7xkk() {}

void Chip8::OP_8xy0() {}

void Chip8::OP_8xy1() {}

void Chip8::OP_8xy2() {}

void Chip8::OP_8xy3() {}

void Chip8::OP_8xy4() {}

void Chip8::OP_8xy5() {}

void Chip8::OP_8xy6() {}

void Chip8::OP_8xy7() {}

void Chip8::OP_8xyE() {}

void Chip8::OP_9xy0() {}

void Chip8::OP_Annn() {}

void Chip8::OP_Bnnn() {}

void Chip8::OP_Cxkk() {}

void Chip8::OP_Dxyn() {}

void Chip8::OP_Ex9E() {}

void Chip8::OP_ExA1() {}

void Chip8::OP_Fx07() {}

void Chip8::OP_Fx0A() {}

void Chip8::OP_Fx15() {}

void Chip8::OP_Fx18() {}

void Chip8::OP_Fx1E() {}

void Chip8::OP_Fx29() {}

void Chip8::OP_Fx33() {}

void Chip8::OP_Fx55() {}

void Chip8::OP_Fx65() {}

// Methods:

void Chip8::loadRom(const char *pFileName) {
    int fileOffsetValue = 0;

    // Open the file name as binary, move to end.
    std::ifstream file(pFileName, std::ios::binary | std::ios::ate);

    if (file.is_open()) {
        // Get Size of file stream.
        std::streampos size = file.tellg();

        // Create and allocate buffer to heap to hold contents of file.
        char* buffer = new char[size];

        // Return to the beginning of the file and write it to the buffer;
        file.seekg(fileOffsetValue, std::ios::beg);
        file.read(buffer, size);
        file.close();

        // Starting at address 0x200, load the rom contents into [mMemory].
        for (long iAddressOffset = 0; iAddressOffset < size; ++iAddressOffset) {
            mMemory[START_ADDRESS + iAddressOffset] = buffer[iAddressOffset];
        }

        delete[] buffer;
    }
}
