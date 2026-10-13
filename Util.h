#ifndef UTIL_H
#define UTIL_H

#define Byte    unsigned char
#define Short   unsigned short int

const Byte BIT0 = 1;
const Byte BIT1 = 2;
const Byte BIT2 = 4;
const Byte BIT3 = 8;
const Byte BIT4 = 16;
const Byte BIT5 = 32;
const Byte BIT6 = 64;
const Byte BIT7 = 128;

class Util
{
private:

public:
    Util();
    ~Util();

    char* HByte(Byte value);
    char* HWord(Short value);
    char* HBool(Byte value);
    int EndianFlip(int value);
};

#endif
