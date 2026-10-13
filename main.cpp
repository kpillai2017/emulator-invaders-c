#include "Emulator.h"

int main(int argc, char *argv[])
{
	// Instantiate the emulator and run
    Emulator emulator;

    if (!emulator.init())
    {
        emulator.free();
        return 0;
    }

    emulator.run();

    return 0;
}
