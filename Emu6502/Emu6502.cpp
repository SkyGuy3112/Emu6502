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
};

struct CPU
{
    Word PC;        // program counter
    Word SP;        // stack pointer

    Byte A, X, Y;   // registers

    Byte C : 1;     // status flag
    Byte Z : 1;     // status flag
    Byte I : 1;     // status flag
    Byte D : 1;     // status flag
    Byte B : 1;     // status flag
    Byte V : 1;     // status flag
    Byte N : 1;     // status flag

    void Reset(Mem &memory)
    {
        PC = 0xFFFC;
        SP = 0x0100;
        C = Z = I = D = B = V = N = 0;
        A = X = Y = 0;
    }

    Byte FetchByte(u32 Cycles, Mem& memory)
    {
        Byte Data = memory[PC];
        PC++;
        Cycles--;
        return Data;
    }

    void Execute(u32 Cycles, Mem& memory)
    {
        while (Cycles > 0)
        {
            Byte Ins = FetchByte(Cycles, memory);
        }
    }
};


int main()
{
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);
    cpu.Execute(2, mem);
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
