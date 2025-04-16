// Emu6502.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

struct CPU
{
    using Byte = unsigned char;
    using Word = unsigned short;

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

    void Reset()
    {
        PC = 0xFFFC;
        SP = 0x0100;
        C = Z = I = D = B = V = N = 0;
        A = X = Y = 0;
    }
};


int main()
{
    CPU cpu;
    cpu.reset();
    std::cout << "Hello World!\n";
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
