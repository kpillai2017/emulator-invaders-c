/**
 *  @file    Cpu.cpp
 *  @author  Kumar Pillai
 *  @date    01/Jan/2017
 *  @version 1.0
 *
 *  @section DESCRIPTION
 *
 *  Initialises and runs the CPU instructions.
 *
 */

#include <cstring>
#include "Cpu.h"
#include "Util.h"

Cpu::Cpu()
{

}

Cpu::~Cpu()
{

}

/**
 *   @brief  Initialises the CPU.
 *
 *   @param  m is the memory ptr
 *   @param  io is the io ptr
 *   @return bool
 */
bool Cpu::init(Memory* m, Io* io)
{
    cpuMemory = m;
    cpuMemoryPtr = m->getMemoryPtr();
    cpuIo = io;

    reset();

    debugOutputPtr = new char[20];
    debugOutputPtr[0] = '\0';
    if (debugOutputPtr == NULL)
        return false;
    return true;
}


/**
 *   @brief  Resets the CPU.
 */
void Cpu::reset()
{
    PC = 0;
    A = 0;
    BC = 0;
    DE = 0;
    HL = 0;
    SIGN = false;
    ZERO = false;
    HALFCARRY = false;
    PARITY = false;
    CARRY = false;
    INTERRUPT = false;
    CRASHED = false;

    count_instructions = 0;
    interrupt_alternate = 0;
}

/**
 *   @brief  Runs the CPU.
 */
void Cpu::run()
{
    // Run about one frame's worth of instruction code
    for (int i=0; i<instruction_per_frame; i++)
    {
        if (CRASHED)
            return;
        executeInstruction();
    }
}

/**
 *   @brief  Execute the instructions.
 */
void Cpu::executeInstruction()
{
    disassembly_pc = PC;
    current_inst = fetchRomByte();

    switch (current_inst)
    {
        case 0x00:
            executeNOP();
            break;

        case 0xc2:
        case 0xc3:
        case 0xca:
        case 0xd2:
        case 0xda:
        case 0xf2:
        case 0xfa:
            executeJMP();
            break;

        case 0x01:
        case 0x11:
        case 0x21:
        case 0x31:
            executeLXI();
            break;

        case 0x06:
        case 0x0e:
        case 0x16:
        case 0x1e:
        case 0x26:
        case 0x2e:
        case 0x36:
        case 0x3e:
            executeMVI();
            break;

        case 0xcd:
        case 0xc4:
        case 0xcc:
        case 0xd4:
        case 0xdc:
            executeCALL();
            break;

        case 0x0a:
        case 0x1a:
        case 0x3a:
            executeLDA();
            break;

        case 0x77:
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x73:
        case 0x74:
        case 0x75:
            executeMOVHL();
            break;

        case 0x03:
        case 0x13:
        case 0x23:
        case 0x33:
            executeINX();
            break;

        case 0x0b:
        case 0x1b:
        case 0x2b:
        case 0x3b:
            executeDCX();
            break;

        case 0x05:
        case 0x0d:
        case 0x15:
        case 0x1d:
        case 0x25:
        case 0x2d:
        case 0x35:
        case 0x3d:
            executeDEC();
            break;

        case 0x04:
        case 0x0c:
        case 0x14:
        case 0x1c:
        case 0x24:
        case 0x2c:
        case 0x34:
        case 0x3c:
            executeINC();
            break;

        case 0xc0:
        case 0xc8:
        case 0xc9:
        case 0xd0:
        case 0xd8:
            executeRET();
            break;

        case 0x78:
        case 0x79:
        case 0x7A:
        case 0x7B:
        case 0x7C:
        case 0x7D:
        case 0x7E:
        case 0x7F:
            executeMOV();
            break;

        case 0x40:
        case 0x41:
        case 0x42:
        case 0x43:
        case 0x44:
        case 0x45:
        case 0x46:
        case 0x47:
            executeMOV();
            break;

        case 0x48:
        case 0x49:
        case 0x4a:
        case 0x4b:
        case 0x4c:
        case 0x4d:
        case 0x4e:
        case 0x4f:
            executeMOV();
            break;

        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
        case 0x55:
        case 0x56:
        case 0x57:
            executeMOV();
            break;

        case 0x58:
        case 0x59:
        case 0x5a:
        case 0x5b:
        case 0x5c:
        case 0x5d:
        case 0x5e:
        case 0x5f:
            executeMOV();
            break;

        case 0x60:
        case 0x61:
        case 0x62:
        case 0x63:
        case 0x64:
        case 0x65:
        case 0x66:
        case 0x67:
            executeMOV();
            break;

        case 0x68:
        case 0x69:
        case 0x6a:
        case 0x6b:
        case 0x6c:
        case 0x6d:
        case 0x6e:
        case 0x6f:
            executeMOV();
            break;

        case 0xb8:
        case 0xb9:
        case 0xba:
        case 0xbb:
        case 0xbc:
        case 0xbd:
        case 0xbe:
        case 0xbf:
        case 0xfe:
            executeCMP();
            break;

        case 0xc5:
        case 0xd5:
        case 0xe5:
        case 0xf5:
            executePUSH();
            break;

        case 0xc1:
        case 0xd1:
        case 0xe1:
        case 0xf1:
            executePOP();
            break;

        case 0x09:
        case 0x19:
        case 0x29:
        case 0x39:
            executeDAD();
            break;

        case 0xeb:
            executeXCHG();
            break;

        case 0xe3:
            executeXTHL();
            break;

        case 0xd3:
            executeOUTP();
            break;

        case 0xdb:
            executeINP();
            break;

        case 0xe9:
            executePCHL();
            break;

        case 0xc7:
        case 0xcf:
        case 0xd7:
        case 0xdf:
        case 0xe7:
        case 0xef:
        case 0xf7:
        case 0xff:
            executeRST();
            break;

        case 0x07:
            executeRLC();
            break;

        case 0x17:
            executeRAL();
            break;

        case 0x0f:
            executeRRC();
            break;

        case 0x1f:
            executeRAR();
            break;

        case 0xa0:
        case 0xa1:
        case 0xa2:
        case 0xa3:
        case 0xa4:
        case 0xa5:
        case 0xa6:
        case 0xe6:
        case 0xa7:
            executeAND();
            break;

        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x86:
        case 0xc6:
        case 0x87:
            executeADD();
            break;

        case 0x02:
        case 0x12:
        case 0x32:
            executeSTA();
            break;

        case 0xa8:
        case 0xa9:
        case 0xaa:
        case 0xab:
        case 0xac:
        case 0xad:
        case 0xae:
        case 0xaf:
        case 0xee:
            executeXOR();
            break;

        case 0xf3:
            executeDI();
            break;

        case 0xfb:
            executeEI();
            break;

        case 0x37:
            executeSTC();
            break;

        case 0x3f:
            executeCMC();
            break;

        case 0xb0:
        case 0xb1:
        case 0xb2:
        case 0xb3:
        case 0xb4:
        case 0xb5:
        case 0xb6:
        case 0xb7:
        case 0xf6:
            executeOR();
            break;

        case 0x90:
        case 0x91:
        case 0x92:
        case 0x93:
        case 0x94:
        case 0x95:
        case 0x96:
        case 0x97:
        case 0xd6:
            executeSUB();
            break;

        case 0x2a:
            executeLHLD();
            break;

        case 0x22:
            executeSHLD();
            break;

        case 0x9e:
            executeSBBM();
            break;

        case 0xde:
            executeSBBI();
            break;

        case 0x27:
            executeDAA();
            break;

        case 0x2f:
            executeCMA();
            break;

        case 0x88:
        case 0x89:
        case 0x8a:
        case 0x8b:
        case 0x8c:
        case 0x8d:
        case 0x8e:
        case 0x8f:
        case 0xce:
            executeADC();
            break;

        default:
            char tmpBuf[50];
            sprintf(tmpBuf, "Undefined instruction: %s", cpuUtil.HByte(current_inst));
            Disassembly(tmpBuf);
            CRASHED = true;
            break;
    }
    count_instructions++;

    if (count_instructions >= half_instruction_per_frame)
    {
        if (INTERRUPT)
        {
            // Two interrupt occur every frame: (address $08 and $10)
            if (interrupt_alternate == 0)
                CallInterrupt(0x08);
            else
                CallInterrupt(0x10);
		}

		interrupt_alternate = 1 - interrupt_alternate;
		count_instructions = 0;
    }
}

void Cpu::CallInterrupt(int inAddress)
{
	INTERRUPT = false;
	pushStack(PC);
	PC = inAddress;
}

void Cpu::executeNOP()
{
    //Disassembly("NOP")
}

void Cpu::executeJMP()
{
    // Local name
    Byte condition = true;
    Short data16 = fetchRomShort();

    switch (current_inst)
    {
        case 0xc3:
            //name = "JMP"
            break;
        case 0xc2:
            //name = "JNZ"
            condition = not ZERO;
            break;
        case 0xca:
            //name = "JZ"
            condition = ZERO;
            break;
        case 0xd2:
            //name = "JNC"
            condition = not CARRY;
            break;
        case 0xda:
            //name = "JC"
            condition = CARRY;
            break;
        case 0xf2:
            //name = "JP"
            condition = not SIGN;
            break;
        case 0xfa:
            //name = "JM"
            condition = SIGN;
            break;
    }

    //Disassembly(name$+" "+HWord$(data16)+"  condition="+condition)
    if (condition)
    {
        PC = data16;
    }
}


void Cpu::executeLXI()
{
    // Local name$
    Short data16 = fetchRomShort();

    switch(current_inst)
    {
        case 0x01:
            //name = "BC"
            SetBC(data16);
            break;

        case 0x11:
            //name = "DE"
            SetDE(data16);
            break;

        case 0x21:
            //name = "HL"
            SetHL(data16);
            break;

        case 0x31:
            //name = "SP"
            SetSP(data16);
            break;
    }
}

void Cpu::executeMVI()
{
	// Local name$
	Byte data8 = fetchRomByte();

	switch(current_inst)
	{
		case 0x3e:
			// name = "A"
			SetA(data8);
			break;

		case 0x06:
			// name = "B"
			SetB(data8);
			break;

		case 0x0e:
			// name = "C"
			SetC(data8);
			break;

		case 0x16:
			// name = "D"
			SetD(data8);
			break;

		case 0x1e:
			// name = "E"
			SetE(data8);
			break;

		case 0x26:
			// name = "H"
			SetH(data8);
			break;

		case 0x2e:
			// name = "L"
			SetL(data8);
			break;

		case 0x36:
			// name0x = "[HL]"
			writeByte(HL, data8);
			break;
	}
	// Disassembly("MVI "+name0x+","+HByte0x(data8))
}

void Cpu::executeCALL()
{
	// Local name0x
	Byte condition = true;
	Short data16 = fetchRomShort();

	switch(current_inst)
	{
		case 0xcd:
			// name = "CALL"
			break;

		case 0xc4:
			// name = "CALL NZ"
			condition = not ZERO;
			break;

		case 0xcc:
			// name = "CALL Z"
			condition = ZERO;
			break;

		case 0xd4:
			// name = "CALL NC"
			condition = not CARRY;
			break;

		case 0xdc:
			// name = "CALL C"
			condition = CARRY;
			break;
	}

	// Disassembly(name0x+" "+HWord0x(data16)+"  condition="+condition)
	if (condition)
	{
		pushStack(PC);
		PC = data16;
	}
}

void Cpu::executeRET()
{
	// Local name0x
	Byte condition = true;

	switch(current_inst)
	{
		case 0xc9:
			// name = "RET"
			break;

		case 0xc0:
			// name = "RET NZ"
			condition = not ZERO;
			break;

		case 0xc8:
			// name = "RET Z"
			condition = ZERO;
			break;

		case 0xd0:
			// name = "RET NC"
			condition = not CARRY;
			break;

		case 0xd8:
			// name = "RET C"
			condition = CARRY;
			break;
	}

	// Disassembly(name0x+"   condition="+condition)
	if (condition)
	{
		PC = popStack();
	}
}

void Cpu::executeLDA()
{
	// Local name0x
	Short source;

	switch(current_inst)
	{
		case 0x0a:
			// name = "BC"
			source = BC;
			break;

		case 0x1a:
			// name = "DE"
			source = DE;
			break;

		case 0x3a:
			source = fetchRomShort();
			break;

			// name = HWord0x(source)
	}
	SetA(readByte(source));
	// Disassembly("LDA ["+name0x+"]")
}

void Cpu::executePUSH()
{
	// Local name0x
	Short value;

	switch(current_inst)
	{
		case 0xc5:
			// name = "BC"
			value = BC;
			break;

		case 0xd5:
			// name = "DE"
			value = DE;
			break;

		case 0xe5:
			// name = "HL"
			value = HL;
			break;

		case 0xf5:
			// name ="AF"
			value = (A << 8);
			if (SIGN) value |= BIT7;
			if (ZERO) value |= BIT6;
			if (INTERRUPT) value |= BIT5;
			if (HALFCARRY) value |= BIT4;
			if (CARRY) value |= BIT0;
			break;
	}

	pushStack(value);

	// Disassembly("PUSH "+name0x)
}

void Cpu::executePOP()
{
	// Local name0x
	Short value = popStack();

	switch(current_inst)
	{
		case 0xc1:
			// name = "BC"
			SetBC(value);
			break;

		case 0xd1:
			// name = "DE"
			SetDE(value);
			break;

		case 0xe1:
			// name = "HL"
			SetHL(value);
			break;

		case 0xf1:
			// name ="AF"
			A = value >> 8;
			SIGN = (value & BIT7);
			ZERO = (value & BIT6);
			INTERRUPT = (value & BIT5);
			HALFCARRY = (value & BIT4);
			CARRY = (value & BIT0);
			break;
	}

	// Disassembly("POP "+name0x)
}

void Cpu::executeMOVHL()
{
	// Local name0x

	switch(current_inst)
	{
		case 0x77:
			// name = "A"
			writeByte(HL, A);
			break;

		case 0x70:
			// name = "B"
			writeByte(HL, B);
			break;

		case 0x71:
			// name = "C"
			writeByte(HL, C);
			break;

		case 0x72:
			// name = "D"
			writeByte(HL, D);
			break;

		case 0x73:
			// name = "E"
			writeByte(HL, E);
			break;

		case 0x74:
			// name = "H"
			writeByte(HL, H);
			break;

		case 0x75:
			// name = "L"
			writeByte(HL, L);
			break;
	}
	// Disassembly("MOV [HL],"+ name0x)
}

void Cpu::executeMOV()
{
	// Local src0x, dst0x

	switch(current_inst)
	{
		case 0x7f:
			// dst = "A"; src = "A"
			SetA(A);
			break;

		case 0x78:
			// dst = "A"; src = "B"
			SetA(B);
			break;

		case 0x79:
			// dst = "A"; src = "C"
			SetA(C);
			break;

		case 0x7a:
			// dst = "A"; src = "D"
			SetA(D);
			break;

		case 0x7b:
			// dst = "A"; src = "E"
			SetA(E);
			break;

		case 0x7c:
			// dst = "A"; src = "H"
			SetA(H);
			break;

		case 0x7d:
			// dst = "A"; src = "L"
			SetA(L);
			break;

		case 0x7e:
			// dst = "A"; src = "[HL]"
			SetA(readByte(HL));
			break;

		case 0x47:
			// dst = "B"; src = "A"
			SetB(A);
			break;

		case 0x40:
			// dst = "B"; src = "B"
			SetB(B);
			break;

		case 0x41:
			// dst = "B"; src = "C"
			SetB(C);
			break;

		case 0x42:
			// dst = "B"; src = "D"
			SetB(D);
			break;

		case 0x43:
			// dst = "B"; src = "E"
			SetB(E);
			break;

		case 0x44:
			// dst = "B"; src = "H"
			SetB(H);
			break;

		case 0x45:
			// dst = "B"; src = "L"
			SetB(L);
			break;

		case 0x46:
			// dst = "B"; src = "[HL]"
			SetB(readByte(HL));
			break;

		case 0x4f:
			// dst = "C"; src = "A"
			SetC(A);
			break;

		case 0x48:
			// dst = "C"; src = "B"
			SetC(B);
			break;

		case 0x49:
			// dst = "C"; src = "C"
			SetC(C);
			break;

		case 0x4a:
			// dst = "C"; src = "D"
			SetC(D);
			break;

		case 0x4b:
			// dst = "C"; src = "E"
			SetC(E);
			break;

		case 0x4c:
			// dst = "C"; src = "H"
			SetC(H);
			break;

		case 0x4d:
			// dst = "C"; src = "L"
			SetC(L);
			break;

		case 0x4e:
			// dst = "C"; src = "[HL]"
			SetC(readByte(HL));
			break;

		case 0x57:
			// dst = "D"; src = "A"
			SetD(A);
			break;

		case 0x50:
			// dst = "D"; src = "B"
			SetD(B);
			break;

		case 0x51:
			// dst = "D"; src = "C"
			SetD(C);
			break;

		case 0x52:
			// dst = "D"; src = "D"
			SetD(D);
			break;

		case 0x53:
			// dst = "D"; src = "E"
			SetD(E);
			break;

		case 0x54:
			// dst = "D"; src = "H"
			SetD(H);
			break;

		case 0x55:
			// dst = "D"; src = "L"
			SetD(L);
			break;

		case 0x56:
			// dst = "D"; src = "[HL]"
			SetD(readByte(HL));
			break;

		case 0x5f:
			// dst = "E"; src = "A"
			SetE(A);
			break;

		case 0x58:
			// dst = "E"; src = "B"
			SetE(B);
			break;

		case 0x59:
			// dst = "E"; src = "C"
			SetE(C);
			break;

		case 0x5a:
			// dst = "E"; src = "D"
			SetE(D);
			break;

		case 0x5b:
			// dst = "E"; src = "E"
			SetE(E);
			break;

		case 0x5c:
			// dst = "E"; src = "H"
			SetE(H);
			break;

		case 0x5d:
			// dst = "E"; src = "L"
			SetE(L);
			break;

		case 0x5e:
			// dst = "E"; src = "[HL]"
			SetE(readByte(HL));
			break;

		case 0x67:
			// dst = "H"; src = "A"
			SetH(A);
			break;

		case 0x60:
			// dst = "H"; src = "B"
			SetH(B);
			break;

		case 0x61:
			// dst = "H"; src = "C"
			SetH(C);
			break;

		case 0x62:
			// dst = "H"; src = "D"
			SetH(D);
			break;

		case 0x63:
			// dst = "H"; src = "E"
			SetH(E);
			break;

		case 0x64:
			// dst = "H"; src = "H"
			SetH(H);
			break;

		case 0x65:
			// dst = "H"; src = "L"
			SetH(L);
			break;

		case 0x66:
			// dst = "H"; src = "[HL]"
			SetH(readByte(HL));
			break;

		case 0x6f:
			// dst = "L"; src = "A"
			SetL(A);
			break;

		case 0x68:
			// dst = "L"; src = "B"
			SetL(B);
			break;

		case 0x69:
			// dst = "L"; src = "C"
			SetL(C);
			break;

		case 0x6a:
			// dst = "L"; src = "D"
			SetL(D);
			break;

		case 0x6b:
			// dst = "L"; src = "E"
			SetL(E);
			break;

		case 0x6c:
			// dst = "L"; src = "H"
			SetL(H);
			break;

		case 0x6d:
			// dst = "L"; src = "L"
			SetL(L);
			break;

		case 0x6e:
			// dst = "L"; src = "[HL]"
			SetL(readByte(HL));
			break;
	}

	//Disassembly("MOV "+dst0x+","+src0x)
}

void Cpu::executeINX()
{
	// Local name0x

	switch(current_inst)
	{
		case 0x03:
			// name = "BC"
			SetBC(BC+1);
			break;

		case 0x13:
			// name = "DE"
			SetDE(DE+1);
			break;

		case 0x23:
			// name = "HL"
			SetHL(HL+1);
			break;

		case 0x33:
			// name = "SP"
			SetSP(SP+1);
			break;
	}

	// Disassembly("INX "+name0x)
}

void Cpu::executeDAD()
{
	// Local name0x

	switch(current_inst)
	{
		case 0x09:
			// name = "BC"
			AddHL(BC);
			break;

		case 0x19:
			// name = "DE"
			AddHL(DE);
			break;

		case 0x29:
			// name = "HL"
			AddHL(HL);
			break;

		case 0x39:
			// name = "SP"
			AddHL(SP);
			break;
	}
	// Disassembly("DAD HL,"+name0x)
}

void Cpu::AddHL(Short inValue)
{
	int value = HL + inValue;
	SetHL(value);

	CARRY = (value > 65535);
}

void Cpu::executeDCX()
{
	// Local name0x

	switch(current_inst)
	{
		case 0x0b:
			// name = "BC"
			SetBC(BC-1);
			break;

		case 0x1b:
			// name = "DE"
			SetDE(DE-1);
			break;

		case 0x2b:
			// name = "HL"
			SetHL(HL-1);
			break;

		case 0x3b:
			// name = "SP"
			SetSP(SP-1);
			break;
	}

	// Disassembly("DCX "+name0x)
}

void Cpu::executeDEC()
{
	// Local name0x

	switch(current_inst)
	{
		case 0x3d:
			// name = "A"
			SetA(PerformDec(A));
			break;

		case 0x05:
			// name = "B"
			SetB(PerformDec(B));
			break;

		case 0x0d:
			// name = "C"
			SetC(PerformDec(C));
			break;

		case 0x15:
			// name = "D"
			SetD(PerformDec(D));
			break;

		case 0x1d:
			// name = "E"
			SetE(PerformDec(E));
			break;

		case 0x25:
			// name = "H"
			SetH(PerformDec(H));
			break;

		case 0x2d:
			// name = "L"
			SetL(PerformDec(L));
			break;

		case 0x35:
			// name = "[HL]"
			Byte data8 = readByte(HL);
			writeByte(HL, PerformDec(data8));
			break;
	}

	// Disassembly("DEC "+ name0x)
}

void Cpu::executeINC()
{
	// Local name0x

	switch(current_inst)
	{
		case 0x3c:
			// name = "A"
			SetA(PerformInc(A));
			break;

		case 0x04:
			// name = "B"
			SetB(PerformInc(B));
			break;

		case 0x0c:
			// name = "C"
			SetC(PerformInc(C));
			break;

		case 0x14:
			// name = "D"
			SetD(PerformInc(D));
			break;

		case 0x1c:
			// name = "E"
			SetE(PerformInc(E));
			break;

		case 0x24:
			// name = "H"
			SetH(PerformInc(H));
			break;

		case 0x2c:
			// name = "L"
			SetL(PerformInc(L));
			break;

		case 0x34:
			// name = "[HL]"
			Byte data8 = readByte(HL);
			writeByte(HL, PerformInc(data8));
			break;
	}

	// Disassembly("INC "+ name0x)
}

void Cpu::executeAND()
{
	// Local name0x

	switch(current_inst)
	{
		case 0xa7:
			// name = "A"
			PerformAnd(A);
			break;

		case 0xa0:
			// name = "B"
			PerformAnd(B);
			break;

		case 0xa1:
			// name = "C"
			PerformAnd(C);
			break;

		case 0xa2:
			// name = "D"
			PerformAnd(D);
			break;

		case 0xa3:
			// name = "E"
			PerformAnd(E);
			break;

		case 0xa4:
			// name = "H"
			PerformAnd(H);
			break;

		case 0xa5:
			// name = "L"
			PerformAnd(L);
			break;

		case 0xa6:
			// name = "[HL]"
			PerformAnd(readByte(HL));
			break;

		case 0xe6:
			Byte immediate = fetchRomByte();
			// name = HByte0x(immediate)
			PerformAnd(immediate);
			break;
	}
	// Disassembly("AND "+ name0x)
}

void Cpu::executeXOR()
{
	// Local name0x

	switch(current_inst)
	{
		case 0xaf:
			// name = "A"
			PerformXor(A);
			break;

		case 0xa8:
			// name = "B"
			PerformXor(B);
			break;

		case 0xa9:
			// name = "C"
			PerformXor(C);
			break;

		case 0xaa:
			// name = "D"
			PerformXor(D);
			break;

		case 0xab:
			// name = "E"
			PerformXor(E);
			break;

		case 0xac:
			// name = "H"
			PerformXor(H);
			break;

		case 0xad:
			// name = "L"
			PerformXor(L);
			break;

		case 0xae:
			// name = "[HL]"
			PerformXor(readByte(HL));
			break;

		case 0xee:
			Byte immediate = fetchRomByte();
			// name = HByte0x(immediate)
			PerformXor(immediate);
			break;
	}
	// Disassembly("XOR "+ name0x)
}

void Cpu::executeOR()
{
	// Local name0x

	switch(current_inst)
	{
		case 0xb7:
			// name = "A"
			PerformOr(A);
			break;

		case 0xb0:
			// name = "B"
			PerformOr(B);
			break;

		case 0xb1:
			// name = "C"
			PerformOr(C);
			break;

		case 0xb2:
			// name = "D"
			PerformOr(D);
			break;

		case 0xb3:
			// name = "E"
			PerformOr(E);
			break;

		case 0xb4:
			// name = "H"
			PerformOr(H);
			break;

		case 0xb5:
			// name = "L"
			PerformOr(L);
			break;

		case 0xb6:
			// name = "[HL]"
			PerformOr(readByte(HL));
			break;

		case 0xf6:
			Byte immediate = fetchRomByte();
			// name = HByte0x(immediate)
			PerformOr(immediate);
			break;
	}
	// Disassembly("OR "+ name0x)
}

void Cpu::executeADD()
{
	// Local name0x

	switch(current_inst)
	{
		case 0x87:
			// name = "A"
			PerformByteAdd(A);
			break;

		case 0x80:
			// name = "B"
			PerformByteAdd(B);
			break;

		case 0x81:
			// name = "C"
			PerformByteAdd(C);
			break;

		case 0x82:
			// name = "D"
			PerformByteAdd(D);
			break;

		case 0x83:
			// name = "E"
			PerformByteAdd(E);
			break;

		case 0x84:
			// name = "H"
			PerformByteAdd(H);
			break;

		case 0x85:
			// name = "L"
			PerformByteAdd(L);
			break;

		case 0x86:
			// name = "[HL]"
			PerformByteAdd(readByte(HL));
			break;

		case 0xc6:
			Byte immediate = fetchRomByte();
			// name = HByte0x(immediate)
			PerformByteAdd(immediate);
			break;
	}
	// Disassembly("ADD "+ name0x)
}

void Cpu::executeADC()
{
	// Local name0x
	Byte carryvalue = 0;
	if (CARRY) carryvalue = 1;

	switch(current_inst)
	{
		case 0x8f:
			// name = "A"
			PerformByteAdd(A, carryvalue);
			break;

		case 0x88:
			// name = "B"
			PerformByteAdd(B, carryvalue);
			break;

		case 0x89:
			// name = "C"
			PerformByteAdd(C, carryvalue);
			break;

		case 0x8a:
			// name = "D"
			PerformByteAdd(D, carryvalue);
			break;

		case 0x8b:
			// name = "E"
			PerformByteAdd(E, carryvalue);
			break;

		case 0x8c:
			// name = "H"
			PerformByteAdd(H, carryvalue);
			break;

		case 0x8d:
			// name = "L"
			PerformByteAdd(L, carryvalue);
			break;

		case 0x8e:
			// name = "[HL]"
			PerformByteAdd(readByte(HL), carryvalue);
			break;

		case 0xce:
			Byte immediate = fetchRomByte();
			// name = HByte0x(immediate)
			PerformByteAdd(immediate, carryvalue);
			break;
	}
	// Disassembly("ADC "+ name0x)
}

void Cpu::executeSUB()
{
	// Local name0x

	switch(current_inst)
	{
		case 0x97:
			// name = "A"
			PerformByteSub(A);
			break;

		case 0x90:
			// name = "B"
			PerformByteSub(B);
			break;

		case 0x91:
			// name = "C"
			PerformByteSub(C);
			break;

		case 0x92:
			// name = "D"
			PerformByteSub(D);
			break;

		case 0x93:
			// name = "E"
			PerformByteSub(E);
			break;

		case 0x94:
			// name = "H"
			PerformByteSub(H);
			break;

		case 0x95:
			// name = "L"
			PerformByteSub(L);
			break;

		case 0x96:
			// name = "[HL]"
			PerformByteSub(readByte(HL));
			break;

		case 0xd6:
			Byte immediate = fetchRomByte();
			// name = HByte0x(immediate)
			PerformByteSub(immediate);
			break;
	}
	// Disassembly("SUB "+ name0x)
}

void Cpu::executeSBBM()
{
	// SBB M (0x9E): A = A - (HL) - CARRY
	Byte carryvalue = 0;
	if (CARRY) carryvalue = 1;
	PerformByteSub(readByte(HL), carryvalue);
	// Disassembly("SBB M")
}

void Cpu::executeSBBI()
{
	Byte immediate = fetchRomByte();
	Byte carryvalue = 0;
	if (CARRY) carryvalue = 1;
	PerformByteSub(immediate, carryvalue);
	// Disassembly("SBBI "+HByte0x(immediate))
}

void Cpu::executeCMP()
{
	// Local name0x
	Byte value = 0;

	switch(current_inst)
	{
		case 0xbf:
			// name = "A"
			value = A;
			break;

		case 0xb8:
			// name = "B"
			value = B;
			break;

		case 0xb9:
			// name = "C"
			value = C;
			break;

		case 0xba:
			// name = "D"
			value = D;
			break;

		case 0xbb:
			// name = "E"
			value = E;
			break;

		case 0xbc:
			// name = "H"
			value = H;
			break;

		case 0xbd:
			// name = "L"
			value = L;
			break;

		case 0xbe:
			// name = "[HL]"
			value = readByte(HL);
			break;

		case 0xfe:
			value = fetchRomByte();
			break;

			// name = HByte0x(value)
	}
	PerformCompSub(value);
	// Disassembly("CMP "+name0x)
}

void Cpu::executeXCHG()
{
	Short temp = DE;
	SetDE(HL);
	SetHL(temp);
	// Disassembly("XCHG DE,HL")
}

void Cpu::executeXTHL()
{
	Byte temp = H;
	SetH(readByte(SP+1));
	writeByte(SP+1, temp);

	temp = L;
	SetL(readByte(SP));
	writeByte(SP, temp);

	// Disassembly("XTHL HL,[SP]")
}

void Cpu::executeOUTP()
{
	Byte port = fetchRomByte();
	cpuIo->outputPort(port, A);

	// Disassembly("OUTP "+HByte0x(port)+" A="+A)
}

void Cpu::executeINP()
{
	Byte port = fetchRomByte();
	SetA(cpuIo->inputPort(port));

	// Disassembly("INP "+HByte0x(port)+" A="+A)
}

void Cpu::executePCHL()
{
	PC = HL;
	// Disassembly("PCHL")
}

void Cpu::executeRST()
{
	// Local name0x
	Short address;

	switch(current_inst)
	{
		case 0xc7:
			// name = "RST0"
			address = 0x0;
			break;

		case 0xcf:
			// name = "RST1"
			address = 0x8;
			break;

		case 0xd7:
			// name = "RST2"
			address = 0x10;
			break;

		case 0xdf:
			// name = "RST3"
			address = 0x18;
			break;

		case 0xe7:
			// name = "RST4"
			address = 0x20;
			break;

		case 0xef:
			// name = "RST5"
			address = 0x28;
			break;

		case 0xf7:
			// name = "RST6"
			address = 0x30;
			break;

		case 0xff:
			// name = "RST7"
			address = 0x38;
			break;
	}
	// Disassembly(name)

	pushStack(PC);
	PC = address;
}

void Cpu::executeRLC()
{
	SetA((A << 1) | (A >> 7));
	CARRY = A & BIT0;

	// Disassembly("RLC")
}

void Cpu::executeRAL()
{
	Byte temp = A;

	SetA(A << 1);
	if(CARRY) SetA(A | BIT0);
	CARRY = temp & BIT7;

	// Disassembly("RAL")
}

void Cpu::executeRRC()
{
	SetA((A >> 1) | (A << 7));
	CARRY = A & BIT7;
	// Disassembly("RRC")
}

void Cpu::executeRAR()
{
	Byte temp = A;

	SetA(A >> 1);
	if(CARRY) SetA(A | BIT7);
	CARRY = temp & BIT0;

	// Disassembly("RAR")
}

void Cpu::executeSTA()
{
	// Local src0x

	switch(current_inst)
	{
		case 0x02:
			// src = "BC"
			writeByte(BC, A);
			break;

		case 0x12:
			// src = "DE"
			writeByte(DE, A);
			break;

		case 0x32:
			Short immediate = fetchRomShort();
			writeByte(immediate, A);
			break;
			// src = HWord0x(immediate)
	}

	// Disassembly("STA "+src0x)
}

void Cpu::executeDI()
{
	INTERRUPT = false;
	// Disassembly("DI")
}

void Cpu::executeEI()
{
	INTERRUPT = true;
	// Disassembly("EI")
}

void Cpu::executeSTC()
{
	CARRY = true;
	// Disassembly("STC")
}

void Cpu::executeCMC()
{
	CARRY = not CARRY;
	// Disassembly("CMC")
}

void Cpu::executeLHLD()
{
	Short immediate = fetchRomShort();
	SetHL(readShort(immediate));
	// Disassembly("LHLD ["+HWord0x(immediate)+"]")
}

void Cpu::executeSHLD()
{
	Short immediate = fetchRomShort();
	writeShort(immediate, HL);
	// Disassembly("SHLD ["+HWord0x(immediate)+"]")
}

void Cpu::executeDAA()
{
	if(((A & 0x0F) > 9) or (HALFCARRY))
	{
		A += 0x06;
		HALFCARRY = true;
	}
	else
	{
		HALFCARRY = false;
	}

	if((A > 0x9F) or (CARRY))
	{
		A += 0x60;
		CARRY = true;
	}
	else
	{
		CARRY = false;
	}

	setFlagZeroSign();
	// Disassembly("DAA")
}

void Cpu::executeCMA()
{
	SetA(A ^ 255);
	// Disassembly("CMA")
}
///////////////////////////////////////
Byte Cpu::fetchRomByte()
{
    Byte b = cpuMemoryPtr[PC];
    PC += 1;
    return(b & 0xFF);
}

Short Cpu::fetchRomShort()
{
    Short s = (cpuMemoryPtr[PC+1]<<8) + (cpuMemoryPtr[PC]);
    PC += 2;
    return(s & 0xFFFF);
}

void Cpu::Disassembly(char* inText)
{
    char tmpBuf[100];
    strcpy(tmpBuf, cpuUtil.HWord(disassembly_pc));
    strcat(tmpBuf, ": ");
    strcat(tmpBuf, inText);
    //sprintf(tmpBuf, "%s%s%s", cpuUtil.HWord(disassembly_pc), ": ", inText);
    strcpy(debugOutputPtr, tmpBuf);
}

void Cpu::SetA(Byte inByte)
{
        A = inByte;
}
void Cpu::SetB(Byte inByte)
{
        B = inByte;
        BC = (B<<8) + C;
}
void Cpu::SetC(Byte inByte)
{
        C = inByte;
        BC = (B<<8) + C;
}
void Cpu::SetD(Byte inByte)
{
        D = inByte;
        DE = (D<<8) + E;
}
void Cpu::SetE(Byte inByte)
{
        E = inByte;
        DE = (D<<8) + E;
}
void Cpu::SetH(Byte inByte)
{
        H = inByte;
        HL = (H<<8) + L;
}
void Cpu::SetL(Byte inByte)
{
        L = inByte;
        HL = (H<<8) + L;
}
void Cpu::SetBC(Short inShort)
{
        BC = inShort;
        B = BC>>8;
        C = BC;
}
void Cpu::SetDE(Short inShort)
{
        DE = inShort;
        D = DE>>8;
        E = DE;
}
void Cpu::SetHL(Short inShort)
{
        HL = inShort;
        H = HL>>8;
        L = HL;
}
void Cpu::SetSP(Short inShort)
{
        SP = inShort;
}

Byte Cpu::readByte(Short inAddress)
{
    return cpuMemoryPtr[inAddress];
}

Short Cpu::readShort(Short inAddress)
{
    return ((cpuMemoryPtr[inAddress+1] << 8) + (cpuMemoryPtr[inAddress]));
}

void Cpu::writeByte(Short inAddress, Byte inByte)
{
    cpuMemoryPtr[inAddress] = inByte;
}

void Cpu::writeShort(Short inAddress, Short inWord)
{
    cpuMemoryPtr[inAddress+1] = inWord >> 8;
    cpuMemoryPtr[inAddress] = inWord;
}

void Cpu::pushStack(Short inValue)
{
    SP -= 2;
    writeShort(SP, inValue);
}

Short Cpu::popStack()
{
    Short temp = readShort(SP);
    SP += 2;
    return temp;
}
///////////////////////////////////////
void Cpu::setFlagZeroSign()
{
	ZERO = (A == 0);
	SIGN = (A&128);
}

void Cpu::PerformAnd(Byte inValue)
{
	SetA(A & inValue);
	CARRY = false;
	HALFCARRY = false;
	setFlagZeroSign();
}

void Cpu::PerformXor(Byte inValue)
{
	SetA(A ^ inValue);
	CARRY = false;
	HALFCARRY = false;
	setFlagZeroSign();
}

void Cpu::PerformOr(Byte inValue)
{
	SetA(A | inValue);
	CARRY = false;
	HALFCARRY = false;
	setFlagZeroSign();
}

void Cpu::PerformByteAdd(Byte inValue, Byte inCarryValue)
{
	int value = A + inValue + inCarryValue;
	HALFCARRY = ((A ^ inValue ^ value) & 0x10);
	SetA(value);

	CARRY = (value > 255);
	setFlagZeroSign();
}

Byte Cpu::PerformInc(Byte inSource)
{
	int value = inSource + 1;

	HALFCARRY = ((value & 0xF) != 0);

	ZERO = ((value & 255) == 0);
	SIGN = (value & 128);
	return value;
}

Byte Cpu::PerformDec(Byte inSource)
{
	int value = inSource - 1;

	HALFCARRY = ((value & 0xF) == 0);

	ZERO = ((value & 255) == 0);
	SIGN = (value & 128);
	return value;
}

void Cpu::PerformByteSub(Byte inValue, Byte inCarryValue)
{
	Byte value = A - inValue - inCarryValue;

	CARRY = ((value >= A) and (inValue | inCarryValue));

	HALFCARRY = ((A ^ inValue ^ value) & 0x10);

	SetA(value);
	setFlagZeroSign();
}

Byte Cpu::PerformCompSub(Byte inValue)
{
	Byte value = A - inValue;

	CARRY = ((value >= A) and (inValue));
	HALFCARRY = ((A ^ inValue ^ value) & 0x10);
	ZERO = (value == 0);
	SIGN = (value & 128);
}

char* Cpu::getDebugOutput()
{
    return(debugOutputPtr);
}

/**
 *   @brief  Frees the CPU.
 */
void Cpu::free()
{
    if(debugOutputPtr != NULL)
    {
        delete[] debugOutputPtr;
    }
}

