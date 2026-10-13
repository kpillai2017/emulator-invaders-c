#ifndef IO_H
#define IO_H

#include <fstream>
#include "Core/Input.h"
#include "Core/Graphics.h"
#include "Core/Image.h"
#include "Util.h"
#include "SoundFX.h"

#define Byte    unsigned char
#define Short   unsigned short int

class Io
{
private:
    Util IoUtil;
    Input* IoInput;
    SoundFX* IoSoundFX;
    Byte OUT_PORT2;
    Byte OUT_PORT3;
    Byte OUT_PORT4LO;
    Byte OUT_PORT4HI;
    Byte OUT_PORT5;
    Byte IN_PORT1;
    Byte IN_PORT2;
public:
    Io();
    ~Io();

    void init(Input* in, SoundFX* sFX);
    void update();
    void outputPort(Byte port, Byte value);
    Byte inputPort(Byte port);
};

#endif

