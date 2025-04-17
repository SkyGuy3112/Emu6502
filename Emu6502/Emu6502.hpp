
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
    Byte& operator[](u32 Address)
    {
        return Data[Address];
    }

    /* Writing 2 bytes*/
    void WriteWord(Word Value, u32 Address, u32& Cycles)
    {
        Data[Address] = Value >> 8;
        Data[Address + 1] = Value & 0xFF;
        Cycles -= 2;
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

    void Reset(Mem& memory)
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

    Word FetchWord(u32& Cycles, Mem& memory)
    {
        // 6502 is little endian
        Word Data = memory[PC];
        PC++;
        Data |= memory[PC] << 8;
        PC++;

        Cycles -= 2;
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
        INS_LDA_ZPX = 0xB5,                  // Load Accumlator Zero Page Indexed opcode
        INS_JSR = 0x20;                        // Jump to Subroutine opcode



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
            case INS_JSR:
            {
                Word SubAddr = FetchWord(Cycles, memory);
                memory.WriteWord(PC - 1, SP, Cycles);
                SP++;
                PC = SubAddr;
                Cycles--;
            }break;
            default:
            {
                printf("Instruction not handled %d\n", Ins);
            }break;
            }
        }
    }
};

