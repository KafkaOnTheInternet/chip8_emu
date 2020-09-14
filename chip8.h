//
// Created by arjun on 9/9/20.
//

#ifndef CHIP8_EMU_CHIP8_H
#define CHIP8_EMU_CHIP8_H

#include <cstdint>
#include <fstream>
#include <random>
#include <chrono>
#include <cstring>

class chip8
{
public:
    uint16_t  opcode;
    uint8_t memory[4096]; //0x1000 locations
    uint8_t   registers[16]; // registers

    uint16_t  idx;
    uint16_t  pc;

    uint8_t  gfx[64*32];
    uint8_t delay_timer;
    uint8_t  sound_timer;

    uint16_t stack[16];
    uint16_t sp;

    uint8_t keypad[16]; // 0x0 - 0xF

    uint8_t fontset[80] =
            {
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

   std::default_random_engine               randGen;
   std::uniform_int_distribution<uint8_t>   randByte;

public:
    void load_rom(std::string filename);
    chip8();

    void OP_00E0();

    void OP_00EE();

    void OP_1nnn();

    void OP_2nnn();

    void OP_Cxkk();

    void OP_Bnnn();

    void OP_8xyE();

    void OP_9xy0();

    void OP_8xy7();

    void OP_8xy6();

    void OP_8xy5();

    void OP_8xy4();

    void OP_8xy3();

    void OP_8xy2();

    void OP_8xy1();

    void OP_8xy0();

    void OP_7xkk();

    void OP_6xkk();

    void OP_5xy0();

    void OP_4xkk();

    void OP_3xkk();

    void OP_Annn();

    void OP_Dxyn();

    void OP_Fx65();

    void OP_Fx55();

    void OP_Fx33();

    void OP_Fx29();

    void OP_Fx1E();

    void OP_Fx18();

    void OP_Fx15();

    void OP_Fx0A();

    void OP_ExA1();

    void OP_Ex9E();

    void OP_Fx07();
};

chip8::chip8() {
    pc = 0x200;
    opcode = 0;
    idx = 0;
    sp = 0;

    for (int i=0; i < 80; i++)
        memory[0x50+i] = fontset[i];

}

void chip8::load_rom(std::string filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (file.is_open())
    {
        std::streampos size = file.tellg();
        char* buffer = new char[size];

        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        for (size_t i = 0; i < size; i++)
            memory[0x200+i] = buffer[i];

        delete [] buffer;

    }
}

void chip8::OP_00E0()
{
    memset(gfx, 0, sizeof(gfx));
}

void chip8::OP_00EE()
{
    --sp;
    pc = stack[sp];
}

void chip8::OP_1nnn()
{
    pc = opcode & 0x0FFFu;
}

void chip8::OP_2nnn()
{
    stack[sp] = pc;
    ++sp;
    pc = opcode & 0x0FFFu;
}

void chip8::OP_3xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    if (registers[Vx] == byte) {
        pc += 2;
    }
}

void chip8::OP_4xkk()
{
    uint8_t  Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t  byte = opcode & 0x00FFu;

    if (registers[Vx] != byte) {
        pc += 2;
    }
}

void chip8::OP_5xy0()
{
    uint8_t Vs = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0U) >> 4u;
    if (registers[Vx] == registers[Vy]) {
        pc += 2;
    }
}

void chip8::OP_6xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;
    registers[Vx] = byte;
}
void chip8::OP_7xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;
    registers[Vx] += byte;
}
void chip8::OP_8xy0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] = registers[Vy];

}
void chip8::OP_8xy1()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] |= registers[Vy];

}
void chip8::OP_8xy2()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] &= registers[Vy];

}
void chip8::OP_8xy3()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] ^= registers[Vy];

}
void chip8::OP_8xy4()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    uint16_t sum = registers[Vx] + registers[Vy];

    if (sum > 255u)
        registers[0xF] = 1;
    else
        registers[0xF] = 0;

    registers[Vx] = sum & 0x00FFu;
}
void chip8::OP_8xy5()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if (registers[Vx] > registers[Vy])
        registers[0xF] = 1;
    else
        registers[0xF] = 0;

    registers[Vx] -= registers[Vy];

}
void chip8::OP_8xy6()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    registers[0xF] = registers[Vx] & 0x0001u;
    registers[Vx] >>= 1u;
}
void chip8:: OP_8xy7()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if (registers[Vy] > registers[Vx])
        registers[0xF] = 1;
    else
        registers[0xF] = 0;
    registers[Vx] = registers[Vy] - registers[Vx];
}

void chip8:: OP_8xyE()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    registers[0xF] = (registers[Vx] & 0x0080u) >> 7u;
    registers[Vx] <<= 1u;
}
void chip8::OP_9xy0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    if (registers[Vy] != registers[Vx])
        pc += 2;
}

void chip8::OP_Annn()
{
    idx = opcode & 0x0FFFu;
}
void chip8::OP_Bnnn()
{
    uint16_t addr = opcode & 0x0FFFu;
    pc = registers[0] + addr;
}
void chip8::OP_Cxkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;
    registers[Vx] = randByte(randGen) & byte;
}
void chip8::OP_Dxyn()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
    uint8_t yPos = registers[Vy] % VIDEO_HEIGHT;

    registers[0xF] = 0;

    for (size_t row = 0; row < height; row++)
    {
        uint8_t spriteByte = memory[idx+row];
        for (size_t col = 0; col < 8; col++) {
            uint8_t spritePixel = spriteByte & (0x80 >> col);
            uint32_t* screenPixel = &gfx[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

            if (spritePixel)
            {
                if (*screenPixel == 0xFFFFFFFF)
                {
                    registers[0xF] = 1;
                }
            }
            *screenPixel ^= 0xFFFFFFFF;

        }
    }

}
void chip8::OP_Ex9E()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t key = registers[Vx];

    if (keypad[key])
        pc += 2;
}
void chip8::OP_ExA1()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t key = registers[Vx];

    if (!keypad[key])
        pc += 2;
}
void chip8::OP_Fx07()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    registers[Vx] = delay_timer;
}
void chip8::OP_Fx0A()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    if (keypad[0])
        registers[Vx] = 0;
    else if(keypad[1])
        registers[Vx] = 1;
    else if(keypad[2])
        registers[Vx] = 2;
    else if(keypad[3])
        registers[Vx] = 3;
    else if(keypad[4])
        registers[Vx] = 4;
    else if(keypad[5])
        registers[Vx] = 5;
    else if(keypad[6])
        registers[Vx] = 6;
    else if(keypad[7])
        registers[Vx] = 7;
    else if(keypad[8])
        registers[Vx] = 8;
    else if(keypad[9])
        registers[Vx] = 9;
    else if(keypad[10])
        registers[Vx] = 10;
    else if(keypad[11])
        registers[Vx] = 11;
    else if(keypad[12])
        registers[Vx] = 12;
    else if(keypad[13])
        registers[Vx] = 13;
    else if(keypad[14])
        registers[Vx] = 14;
    else if(keypad[15])
        registers[Vx] = 15;
    else
        pc -= 2;
}
void chip8::OP_Fx15()
{
    delay_timer = registers[(opcode & 0x0F00u) >> 8u];

}
void chip8::OP_Fx18()
{
    sound_timer = registers[(opcode & 0x0F00u) >> 8u];
}
void chip8::OP_Fx1E()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    idx += registers[Vx];
}
void chip8::OP_Fx29()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t digit = registers[Vx];
    index = FONTSET_START_ADDRESS + (5 * digit);
}
void chip8::OP_Fx33()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t value = registers[Vx];

    memory[idx+2] = value % 10;
    value /= 10;

    memory[idx+1] = value % 10;
    value /= 10;

    memory[idx+1] = value % 10;
}
void chip8::OP_Fx55()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    for (size_t i = 0; i <= Vx; i++) {
        memory[idx+i] = registers[i];
    }
}
void chip8::OP_Fx65() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    for (size_t i = 0; i <= Vx; i++) {
        registers[i] = memory[idx + i];
    }
}




#endif //CHIP8_EMU_CHIP8_H
