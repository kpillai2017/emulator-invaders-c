#include "Emulator.h"

Emulator::Emulator()
{
}

Emulator::~Emulator()
{
}

bool Emulator::init()
{
    if(!initSystem("Space Invaders Emulator", 800, 600, false))
        return false;

/*
    if(!emulOutlineFont.load("fonts/blitz.fon", 14))
        return false;
*/
    if(!emulMemory.init("data/invaders.rom"))
        return false;

    if(!emulCpu.init(getMemory(), getIo()))
        return false;

    if(!emulSoundFX.init())
        return false;

    if (!emulVideo.init(getGraphics(), getMemory()))
        return false;

    emulIO.init(getInput(), getSoundFX());


    setFPS(60);
    getGraphics()->clear(0, 0, 0);

    return true;
}

void Emulator::update()
{
    Input* in = getInput();

    if(in->keyDown(SDLK_ESCAPE))
        Emulator::end();

    emulIO.update();

    char tmpBuf[50];
    strcpy(tmpBuf, "-> ");
    strcat(tmpBuf, emulCpu.getDebugOutput());

    emulVideo.drawOutlineFont(tmpBuf, 0, 20, 255, 255, 255);
    emulCpu.run();

    emulVideo.draw();
}

void Emulator::draw(Graphics* g)
{

}
SoundFX* Emulator::getSoundFX()
{
    return &emulSoundFX;
}

Memory* Emulator::getMemory()
{
    return &emulMemory;
}

Video* Emulator::getVideo()
{
    return &emulVideo;
}

Io* Emulator::getIo()
{
    return &emulIO;
}


void Emulator::free()
{
    //background.free();
    emulVideo.free();
    emulCpu.free();
    emulSoundFX.free();
    emulMemory.free();
    freeSystem();
}
