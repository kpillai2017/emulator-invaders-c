/** 
 *  @file    Cpu.h
 *  @author  Kumar Pillai
 *  @date    01/Jan/2017  
 *  @version 1.0 
 *  
 *  @section DESCRIPTION
 *  
 * CPU class header.
 *
 */

#ifndef CPU_H
#define CPU_H

#include <fstream>
#include "Core/Graphics.h"
#include "Core/Image.h"
#include "Memory.h"
#include "Io.h"
#include "Util.h"

#define Byte    unsigned char
#define Short   unsigned short int

class Cpu
{
private:
    Util cpuUtil;
    Memory* cpuMemory;
    Byte* cpuMemoryPtr;
    Io* cpuIo;

    Short PC;	//Program Counter: Current instruction pointer. 16-bit register.
    Short SP;	//Stack Pointer. 16-bit register
    Byte A;	//Accumulator. 8-bit register
    Byte B;	//Register B. 8-bit register
    Byte C;	//Register C. 8-bit register
    Byte D;	//Register D. 8-bit register
    Byte E;	//Register E. 8-bit register
    Byte H;	//Register H. 8-bit register
    Byte L;	//Register L. 8-bit register
    Short BC;	//Virtual register BC (16-bit) combination of registers B and C
    Short DE;	//Virtual register DE (16-bit) combination of registers D and E
    Short HL;	//Virtual register HL (16-bit) combination of registers H and L
    Byte SIGN;	//Sign flag
    Byte ZERO;	//Zero flag
    Byte HALFCARRY;	//Half-carry (or Auxiliary Carry) flag
    Byte PARITY;	//Parity flag
    Byte CARRY;	//Carry flag
    Byte INTERRUPT;	//Interrupt Enabled flag
    Byte CRASHED;	//Special flag that tells if CPU is crashed (i.e.: stopped)

    const int  instruction_per_frame = 4000; // Approximate real machine speed
    Byte current_inst;

    // Interrupt handling
    int interrupt_alternate;
    int count_instructions;
    int half_instruction_per_frame = instruction_per_frame >> 1;

    // Additional debug fields, not used by CPU
    Short disassembly_pc;
    char* debugOutputPtr;

public:
    Cpu();
    ~Cpu();

    bool init(Memory* m, Io* io);
    void reset();
    void free();

    void run();

    void SetA(Byte inByte);
    void SetB(Byte inByte);
    void SetC(Byte inByte);
    void SetD(Byte inByte);
    void SetE(Byte inByte);
    void SetH(Byte inByte);
    void SetL(Byte inByte);
    void SetBC(Short inShort);
    void SetDE(Short inShort);
    void SetHL(Short inShort);
    void SetSP(Short inShort);
    void AddHL(Short inValue);

    char* getDebugOutput();
    Byte readByte(Short inAddress);
    Short readShort(Short inAddress);
    void writeByte(Short inAddress, Byte inByte);
    void writeShort(Short inAddress, Short inWord);
    void pushStack(Short inValue);
    Short popStack();
	void CallInterrupt(int inAddress);

    void executeInstruction();
    Byte fetchRomByte();
    Short fetchRomShort();
    void Disassembly(char* inText);

    void executeNOP();
    void executeJMP();
    void executeLXI();
    void executeMVI();
    void executeCALL();
    void executeRET();
    void executeLDA();
    void executePUSH();
    void executePOP();
    void executeMOVHL();
    void executeMOV();
    void executeINX();
    void executeDAD();
    void executeDCX();
    void executeDEC();
    void executeINC();
    void executeAND();
    void executeXOR();
    void executeOR()  ;
    void executeADD();
    void executeADC();
    void executeSUB();
    void executeSBBM();
    void executeSBBI();
    void executeCMP();
    void executeXCHG();
    void executeXTHL();
    void executeOUTP();
    void executeINP();
    void executePCHL();
    void executeRST();
    void executeRLC();
    void executeRAL();
    void executeRRC();
    void executeRAR();
    void executeSTA();
    void executeDI();
    void executeEI();
    void executeSTC();
    void executeCMC();
    void executeLHLD();
    void executeSHLD();
    void executeDAA();
    void executeCMA();

    void setFlagZeroSign();
    void PerformAnd(Byte inValue);
    void PerformXor(Byte inValue);
    void PerformOr(Byte inValue);
    void PerformByteAdd(Byte inValue, Byte inCarryValue=0);
    Byte PerformInc(Byte inSource);
    Byte PerformDec(Byte inSource);
    void PerformByteSub(Byte inValue, Byte inCarryValue=0);
    Byte PerformCompSub(Byte inValue);
};

#endif
