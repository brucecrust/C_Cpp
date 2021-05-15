#ifndef CHIP8_CHIP8_H
#define CHIP8_CHIP8_H

#include <cstdint>
#include <fstream>
#include <chrono>
#include <random>
#include <cstring>

class Chip8 {
public:
    // Constructor:
    Chip8() : mRandGen(std::chrono::system_clock::now().time_since_epoch().count()) {
        // Set the Program Counter to the start address of 0x200.
        mPC = START_ADDRESS;

        // Load fontset into memory.
        for (unsigned int iAddressOffset = 0; iAddressOffset < FONTSET_SIZE; ++iAddressOffset) {
            mMemory[FONTSET_START_ADDRESS + iAddressOffset] = mFontset[iAddressOffset];
        }

        // Get random byte random number between 0 and 255.
        mRandByte = std::uniform_int_distribution<uint8_t>(LOWER_RANDOM_BYTE, UPPER_RANDOM_BYTE);
    }

    // Constants and Members:

    static const unsigned int   START_ADDRESS = 0x200;
    static const unsigned int   FONTSET_START_ADDRESS = 0x50;
    static const unsigned int   FONTSET_SIZE = 80;

    static const uint8_t        LOWER_RANDOM_BYTE = 0;
    static const uint8_t        UPPER_RANDOM_BYTE = 255U;

    uint8_t     mRegisters[16]{};
    uint8_t     mMemory[4096]{};
    uint16_t    mIndex{};
    uint16_t    mPC{};
    uint16_t    mStack[16]{};
    uint8_t     mSP{};
    uint8_t     mDelayTimer{};
    uint8_t     mSoundTimer{};
    uint8_t     mKeypad[16]{};
    uint32_t    mVideo[64 * 32]{};
    uint16_t    mOpcode;

    uint8_t     mFontset[FONTSET_SIZE] = {
                    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                    0x20, 0x60, 0x20, 0x20, 0x70, // 1
                    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
            };

    std::default_random_engine              mRandGen;
    std::uniform_int_distribution<uint8_t>  mRandByte;

    // Opcode Methods:

    void OP_00E0(); // CLS:                     Clear the display.
    void OP_00EE(); // RET:                     Return from subroutine.
    void OP_1nnn(); // JP:                      Jump to location nnn.
    void OP_2nnn(); // CALL:                    Call subroutine at nnn.
    void OP_3xkk(); // SE Vx, byte:             Skip next instruction if Vx == kk.
    void OP_4xkk(); // SNE Vx, byte:            Skip next instruction if Vx != kk.
    void OP_5xy0(); // SE Vx, Vy:               Skip next instruction if Vx == Vy.
    void OP_6xkk(); // LD Vx, byte:             Set Vx = kk.
    void OP_7xkk(); // ADD Vx, byte:            Set Vx = Vx + kk.
    void OP_8xy0(); // LD Vx, Vy:               Set Vx = Vy.
    void OP_8xy1(); // OR Vx, Vy:               Set Vx = Vx OR Vy.
    void OP_8xy2(); // AND Vx, Vy:              Set Vx = Vx AND Vy.
    void OP_8xy3(); // XOR Vx, Vy:              Set Vx = Vx XOR Vy.
    void OP_8xy4(); // ADD Vx, Vy:              Set Vx = Vx + Vy, set VF = carry.
    void OP_8xy5(); // SUB Vx, Vy:              Set Vx = Vx - Vy, set VF = NOT borrow.
    void OP_8xy6(); // SHR Vx:                  Set Vx = Vx SRH 1.
    void OP_8xy7(); // SUBN Vx, Vy:             Set Vx = Vy - Vx, set VF = NOT borrow.
    void OP_8xyE(); // SHL Vx {, Vy}:           Set Vx = Vx SHL 1.
    void OP_9xy0(); // SNE Vx, Vy:              Skip next instruction if Vx != Vy.
    void OP_Annn(); // LD I, addr:              Set I = nnn.
    void OP_Bnnn(); // JP V0, addr:             Jump to location nnn + V0.
    void OP_Cxkk(); // RND Vx, byte:            Set Vx to random byte AND kk.
    void OP_Dxyn(); // DRW Vx, Vy, nibble:      Display n-byte at location I at (Vx, Vy), set VF = collision.
    void OP_Ex9E(); // SKP Vx:                  Skip next instruction if key w/ value of Vx is pressed.
    void OP_ExA1(); // SKNP Vx:                 Skip next instruction if key w/ value of Vx is not pressed.
    void OP_Fx07(); // LD Vx, DT:               Set Vx = delay timer value.
    void OP_Fx0A(); // LD Vx, K:                Wait for key press, story value in Vx.
    void OP_Fx15(); // LD DT, Vx:               Set delay timer = Vx.
    void OP_Fx18(); // LD ST, Vx:               Set sound timer = Vx.
    void OP_Fx1E(); // ADD I, Vx:               Set I = I + Vx.
    void OP_Fx29(); // LD F, Vx:                Set I = location of sprite for digit Vx.
    void OP_Fx33(); // LD B, Vx:                Store BCD representation of Vx in memory location I, I+1, I+2.
    void OP_Fx55(); // LD [I], Vx:              Store registers V0 to Vx in memory at location I.
    void OP_Fx65(); // LD Vx, [I]               Read registers V0 to Vx from memory at location I.

    // Methods:

    void loadRom(char const* pFileName);

    uint8_t getVx();

    uint8_t getVy();

    uint8_t getByte();

    uint16_t getnnn();
};

#endif
