#ifndef SOUND_H
#define SOUND_H

#include "Audio.h"

class Sound
{
private:
    Mix_Chunk* sound;
public:
    Sound();
    ~Sound();
    bool load(char fileName[]);
    void free();
    void pause(int channel);
    void resume(int channel);
    void halt(int channel);

    int play(int loops = 0, int channel = -1);

    bool isLoaded();
};

#endif
