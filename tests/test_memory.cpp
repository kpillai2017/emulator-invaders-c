// Synthetic ROM tests. Compile from project root:
// c++ -std=c++98 -I. Memory.cpp tests/test_memory.cpp -o tmp_rovo_memory_test
// ./tmp_rovo_memory_test; rm tmp_rovo_memory_test
#include "Memory.h"
#include <cassert>
#include <cstdio>

static void writeRom(const char* path, unsigned int size)
{
    FILE* file = fopen(path, "wb");
    assert(file != NULL);
    for (unsigned int i = 0; i < size; ++i)
        fputc(i % 256, file);
    assert(fclose(file) == 0);
}

int main()
{
    const char* path = "tmp_rovo_synthetic.rom";
    Memory memory;
    assert(memory.getMemoryPtr() == NULL);
    assert(!memory.init("tmp_rovo_nonexistent_directory/missing.rom"));
    const unsigned int invalidSizes[] = {0, 8191, 8193, 32769};
    for (unsigned int i = 0; i < 4; ++i)
    {
        writeRom(path, invalidSizes[i]);
        assert(!memory.init(path));
        assert(memory.getMemoryPtr() == NULL);
    }
    writeRom(path, 8192);
    assert(memory.init(path));
    for (unsigned int i = 0; i < 8192; ++i)
        assert(memory.getMemoryPtr()[i] == i % 256);
    for (unsigned int i = 8192; i < 16384; ++i)
        assert(memory.getMemoryPtr()[i] == 0);
    writeRom(path, 8193);
    assert(!memory.init(path));
    assert(memory.getMemoryPtr()[1] == 1);
    writeRom(path, 8192);
    assert(memory.init(path));
    memory.free();
    memory.free();
    assert(memory.getMemoryPtr() == NULL);
    assert(remove(path) == 0);
    puts("C++ ROM loader tests passed.");
}
