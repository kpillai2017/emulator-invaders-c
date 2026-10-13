#include <cstdio>
#include <cstring>
#include "Util.h"

Util::Util()
{

}

Util::~Util()
{

}

char* Util::HByte(Byte value)
{
    char buffer[5];
    sprintf(buffer, "%02X", (value & 0xFF));
	return(buffer);
}

char* Util::HWord(Short value)
{
    char buffer[10];
    sprintf(buffer, "%04X", (value & 0xFFFF));
    return(buffer);
}

char* Util::HBool(Byte value)
{
    char buffer[10];

    strcpy(buffer, value!=0 ? "True" : "False");
    return(buffer);
}

int Util::EndianFlip(int value)
{
  // Depending on CPU endianness, swap (or not) a 32-bit value
  // LittleEndian ' Intel x86 are littleendian: we need to swap byte order
	int temp;
	Byte* src = (Byte*)&value;
	Byte* dst = (Byte*)&temp;

	dst[0] = src[3];
	dst[1] = src[2];
	dst[2] = src[1];
	dst[3] = src[0];

	return(temp);
}

