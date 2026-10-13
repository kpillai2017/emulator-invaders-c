#ifndef EMULATOR_H
#define EMULATOR_H

#include "Core/Game.h"
#include "Memory.h"
#include "Io.h"
#include "Cpu.h"
#include "SoundFX.h"
#include "Video.h"

class Emulator : public Game
{
private:
    Io emulIO;
    Memory emulMemory;
    Cpu emulCpu;
    SoundFX emulSoundFX;
    //Image background;
//    OutlineFont emulOutlineFont;
    Video emulVideo;
    int mapX, mapY;

    static const int SCROLL_SPEED = 10;
public:
    Emulator();
    ~Emulator();
    bool init();
    void update();
    void draw(Graphics* g);
    SoundFX* getSoundFX();
    Memory* getMemory();
    Video* getVideo();
    Io* getIo();
    void free();
};

#endif
