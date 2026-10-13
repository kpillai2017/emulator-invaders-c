#ifndef SOUNDFX_H
#define SOUNDFX_H

#include "Core/Sound.h"
#include <fstream>

const int SOUND_CH_UNDEF    = -2;

class SoundFX
{
private:
    Sound coin;
    Sound baseHit;
    Sound invHit;
    Sound shot;
    Sound ufo;
    Sound ufoHit;
    Sound walk1;
    Sound walk2;
    Sound walk3;
    Sound walk4;
    Sound beginPlay;
    Sound extraLife;

    int coinChannel;
    int baseHitChannel;
    int invHitChannel;
    int shotChannel;
    int ufoChannel = SOUND_CH_UNDEF;
    int walk1Channel;
    int walk2Channel;
    int walk3Channel;
    int walk4Channel;
    int beginPlayChannel;
    int extraLifeChannel;
public:
    SoundFX();
    ~SoundFX();

    bool init();

    void Play();
    void PlayCoin();
    void PlayBaseHit();
    void PlayInvHit();
    void PlayShot();
    void StartUfo();
    void StopUfo();
    void PlayUfoHit();
    void PlayWalk1();
    void PlayWalk2();
    void PlayWalk3();
    void PlayWalk4();
    void PlayBeginPlay();
    void PlayExtraLife();

//    void draw(int xOffset, int yOffset, Graphics* g);
    void free();
};

#endif

