#include <cstdio>
#include <new>
#include "Memory.h"

Memory::Memory() : memoryPtr(NULL)
{
}

Memory::~Memory()
{
    free();
}

bool Memory::init(const char* romFileSpec)
{
    FILE* in = fopen(romFileSpec, "rb");
    if (in == NULL)
    {
        fprintf(stderr, "Cannot open ROM '%s'. Supply your own legally obtained 8192-byte Space Invaders ROM; see README.md.\n", romFileSpec);
        return false;
    }

    Byte rom[8192];
    size_t size = fread(rom, 1, sizeof(rom), in);
    int extra = fgetc(in);
    bool valid = size == sizeof(rom) && extra == EOF && !ferror(in);
    fclose(in);
    if (!valid)
    {
        fprintf(stderr, "Invalid ROM '%s': expected exactly 8192 bytes; see README.md.\n", romFileSpec);
        return false;
    }

    Byte* loaded = new (std::nothrow) Byte[16384]();
    if (loaded == NULL)
        return false;
    for (size_t i = 0; i < sizeof(rom); ++i)
        loaded[i] = rom[i];
    free();
    memoryPtr = loaded;
    return true;
}

Byte* Memory::getMemoryPtr()
{
    return memoryPtr;
}

void Memory::free()
{
    delete[] memoryPtr;
    memoryPtr = NULL;
}
