#include "Sound.h"

Sound::Sound()
{
    sound = NULL;
}

Sound::~Sound()
{

}

bool Sound::load(char fileName[])
{
    sound = Mix_LoadWAV(fileName);

    if(sound == NULL)
        return false;

    return true;
}

void Sound::free()
{
    if(sound != NULL)
    {
        Mix_FreeChunk(sound);
        sound = NULL;
    }
}

int Sound::play(int loops, int channel)
{
    if(sound != NULL)
    {
        return Mix_PlayChannel(channel, sound, loops);
    }
    else
    {
        return -1;
    }
}

void Sound::pause(int channel)
{
    Mix_Pause(channel);
}

void Sound::resume(int channel)
{
    Mix_Resume(channel);
}

void Sound::halt(int channel)
{
    Mix_HaltChannel(channel);
}

bool Sound::isLoaded()
{
    return (sound != NULL);
}
