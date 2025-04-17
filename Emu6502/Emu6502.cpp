// Emu6502.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;

struct Mem
{
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void Initialize()
    {
        for (u32 i = 0; i < MAX_MEM; i++)
        {
            Data[i] = 0;
        }
    }
    
    /* Read Byte from memory Operator */
    Byte operator[](u32 Address) const
    {
        return Data[Address];
    }

    /* Write Byte in memory Operator */
    Byte &operator[](u32 Address)
    {
        return Data[Address];
    }
};

struct CPU
{
    Word PC;        // program counter
    Word SP;        // stack pointer

    Byte A, X, Y;   // registers

    Byte C : 1;     // status flag: Carry Flag
    Byte Z : 1;     // status flag: Zero Flag
    Byte I : 1;     // status flag: Interrupt Flag
    Byte D : 1;     // status flag: Decimal Mode Flag
    Byte B : 1;     // status flag: Break Command
    Byte V : 1;     // status flag: Overflow Flag
    Byte N : 1;     // status flag: Negative (signed) flag

    void Reset(Mem &memory)
    {
        PC = 0xFFFC;
        SP = 0x0100;
        C = Z = I = D = B = V = N = 0;
        A = X = Y = 0;
    }

    Byte FetchByte(u32& Cycles, Mem& memory)
    {
        Byte Data = memory[PC];
        PC++;
        Cycles--;
        return Data;
    }

    Byte ReadByte(u32& Cycles, Byte Address, Mem& memory)
    {
        Byte Data = memory[Address];
        Cycles--;
        return Data;
    }

    // opcodes
    static constexpr Byte INS_LDA_IM = 0xA9, // Load Accumlator Immidiate mode opcode
        INS_LDA_ZP = 0xA5,                   // Load Accumlator Zero Page opcode
        INS_LDA_ZPX = 0xB5;                  // Load Accumlator Zero Page Indexed opcode



    void LDASetStatus()
    {
        Z = (A == 0);
        N = (A & 0b10000000);
    }

    void Execute(u32 Cycles, Mem& memory)
    {
        while (Cycles > 0)
        {
            Byte Ins = FetchByte(Cycles, memory);
            switch (Ins)
            {
                case INS_LDA_IM:
                {
                    Byte Value = FetchByte(Cycles, memory);
                    A = Value;
                    LDASetStatus();
                }break;
                case INS_LDA_ZP:
                {
                    Byte ZeroPageAddress = FetchByte(Cycles, memory);
                    A = ReadByte(Cycles, ZeroPageAddress, memory);
                    LDASetStatus();
                }break;
                case INS_LDA_ZPX:
                {
                    Byte ZeroPageAddress = FetchByte(Cycles, memory);
                    ZeroPageAddress += X;
                    Cycles--;
                    A = ReadByte(Cycles, ZeroPageAddress, memory);
                    LDASetStatus();
                }break;
                default:
                {
                    printf("Instruction not handled %d\n", Ins);
                }break;
            }
        }
    }
};


int main()
{
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);
    
    /* start - inline test program */
    mem[0xFFFC] = CPU::INS_LDA_ZP;
    mem[0xFFFD] = 0x42;
    mem[0x0042] = 0x84;
    /* end - inline test program */
    cpu.Execute(3, mem);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
