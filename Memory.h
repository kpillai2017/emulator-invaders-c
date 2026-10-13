#ifndef MEMORY_H
#define MEMORY_H

#define Byte    unsigned char
#define Short   unsigned short int

class Memory
{
private:
    Byte* memoryPtr;
public:
    Memory();
    ~Memory();

    bool init(const char* romfileSpec);
    Byte* getMemoryPtr();
    void free();
};

#endif
