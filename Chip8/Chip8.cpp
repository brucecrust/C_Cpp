#include "Chip8.h"

// Opcode Methods:

void Chip8::OP_00E0() {
    int value = 0;
    memset(mVideo, value, sizeof(mVideo));
}

void Chip8::OP_00EE() {
    // Since PC += 2, minus one from the SP and set the PC to the previous item in the stack.
    --mSP;
    mPC = mStack[mSP];
}

void Chip8::OP_1nnn() {
    mPC = getnnn();
}

void Chip8::OP_2nnn() {
    // Put the PC on top of the stack, then increment by 1.
    mStack[mSP] = mPC;
    ++mSP;
    // PC is then set to nnn,
    mPC = getnnn();
}

void Chip8::OP_3xkk() {
    // If Vx is set to kk, skip the next instruction.
    if (mRegisters[getVx()] == getByte()) {
        mPC += 2;
    }
}

void Chip8::OP_4xkk() {
    // If Vx is NOT set to kk, skip the next instruction.
    if (mRegisters[getVx()] != getByte()) {
        mPC += 2;
    }
}

void Chip8::OP_5xy0() {
    // If Vx = Vy, skip the instruction.
    if (mRegisters[getVx()] == mRegisters[getVy()]) {
        mPC += 2;
    }
}

void Chip8::OP_6xkk() {
    mRegisters[getVx()] = getByte();
}

void Chip8::OP_7xkk() {
    mRegisters[getVx()] = getByte();
}

void Chip8::OP_8xy0() {
    mRegisters[getVx()] = mRegisters[getVy()];
}

void Chip8::OP_8xy1() {
    mRegisters[getVx()] |= mRegisters[getVy()];
}

void Chip8::OP_8xy2() {
    mRegisters[getVx()] &= mRegisters[getVy()];
}

void Chip8::OP_8xy3() {
    mRegisters[getVx()] ^= mRegisters[getVy()];
}

void Chip8::OP_8xy4() {
    auto Vx = getVx();
    auto sum = mRegisters[Vx] + mRegisters[getVy()];

    // If the sum is greater than 8 bits, carry the one to the lower bit.
    if (sum > 255u) {
        mRegisters[0xF] = 1;
    } else {
        mRegisters[0xF] = 0;
    }

    mRegisters[Vx] = sum & 0xFFu;
}

void Chip8::OP_8xy5() {
    auto Vx = getVx();
    auto Vy = getVy();

    // Do not borrow when subtracting; if Vx > Vy, set the lower bit to 1.
    if (mRegisters[Vx] > mRegisters[Vy]) {
        mRegisters[0xF] = 1;
    } else {
        mRegisters[0xF] = 0;
    }

    mRegisters[Vx] -= mRegisters[Vy];
}

void Chip8::OP_8xy6() {
    auto Vx = getVx();

    // If the lower bit is set to 1, set F to 1.
    mRegisters[0xF] = mRegisters[Vx] & 0x1u;

    mRegisters[Vx] >>= 1;
}

void Chip8::OP_8xy7() {
    auto Vx = getVx();
    auto Vy = getVy();

    // If Vy greater than Vx, set the F to 1.
    if (Vy > Vx) {
        mRegisters[0xF] = 1;
    } else {
        mRegisters[0xF] = 0;
    }

    mRegisters[Vx] = mRegisters[Vy] - mRegisters[Vx];
}

void Chip8::OP_8xyE() {
    auto Vx = getVx();

    mRegisters[0xF] = (mRegisters[Vx] & 0x80u) >> 7u;

    mRegisters[Vx] <<= 1;
}

void Chip8::OP_9xy0() {
    if (mRegisters[getVx()] != mRegisters[getVy()]) {
        mPC += 2;
    }
}

void Chip8::OP_Annn() {
    mIndex = getnnn();
}

void Chip8::OP_Bnnn() {
    mPC = mRegisters[0] + getnnn();
}

void Chip8::OP_Cxkk() {
    mRegisters[getVx()] = mRandByte(mRandGen) & getByte();
}

void Chip8::OP_Dxyn() {
    
}

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

/**
 * Read a given rom from pFileName and load it at the starting address of 0x200.
 * */
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

/**
 * From a given opcode, get the x (lower 4 bits of the upper byte).
 * & the opcode with 0F00 to mask the bits and then right shift to get the value.
 * For example:
 * Take FA18, & with 0F00 to get the value of 0A00, then right shift by 8 bits get 000A, or the value of x.
 * 1111 1010 0001 1000 & 0000 1111 0000 0000 = 0000 1010 0000 0000; >> 8 = 0000 0000 0000 1010.
 * */
uint8_t Chip8::getVx() {
    return (mOpcode & 0x0F00u) >> 8u;
}

/**
 * From a given opcode, get the y (upper 4 bits of the lower byte).
 * & the opcode with 00F0 to mask the bits and then right shift to get the value.
 * For example:
 * Take 1A2B, & with 00F0 to get the value of 0020, then right shift by 4 bits to get 0002, or the value of y.
 * 0001 1010 0010 1011 & 0000 0000 1111 0000 = 0000 0000 0010 0000; >> 4 = 0000 0000 0000 0010.
 * */
uint8_t Chip8::getVy() {
    return (mOpcode & 0x00F0u) >> 4u;
}

/**
 * From a given opcode, get the kk / byte (the lower byte).
 * & the opcode with 00FF to mask the bits.
 * For example:
 * Take FFA8, & with 00FF to get the value of 00A8.
 * 1111 1111 1010 1000 & 0000 0000 1111 1111 = 0000 0000 1010 1000.
 * */
uint8_t Chip8::getByte() {
    return mOpcode & 0x00FFu;
}

/**
 * From a given opcode, get nnn (the lower 12 bits in a given address).
 * & the opcode with 0FFF to mask the bits.
 * For example:
 * Take DFA5, & with 0FFF to get the value of 0FA5.
 * 1101 1111 1010 0101 & 0000 1111 1111 1111 = 0000 1111 1010 0101.
 * */
uint16_t Chip8::getnnn() {
    return mOpcode & 0x0FFFu;
}
