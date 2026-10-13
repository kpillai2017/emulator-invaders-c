#include "SoundFX.h"

SoundFX::SoundFX()
{
}

SoundFX::~SoundFX()
{

}

// All the recorded sound effects
bool SoundFX::init()
{
        if (!coin.load("data/coin.wav"))
            return false;
        if (!baseHit.load("data/basehit.wav"))
            return false;
        if (!invHit.load("data/invhit.wav"))
            return false;
        if (!shot.load("data/shot.wav"))
            return false;
        if (!ufo.load("data/ufo.wav"))
            return false;
        if (!ufoHit.load("data/ufohit.wav"))
            return false;
        if (!walk1.load("data/walk1.wav"))
            return false;
        if (!walk2.load("data/walk2.wav"))
            return false;
        if (!walk3.load("data/walk3.wav"))
            return false;
        if (!walk4.load("data/walk4.wav"))
            return false;
        if (!beginPlay.load("data/beginplay.wav"))
            return false;
        if (!extraLife.load("data/extralife.wav"))
            return false;

    return true;
}

void SoundFX::Play()
{
    baseHit.play(0);
}

void SoundFX::PlayCoin()
{
    coin.play(0);
}

void SoundFX::PlayBaseHit()
{
    baseHit.play(0);
}

void SoundFX::PlayInvHit()
{
    invHit.play(0);
}

void SoundFX::PlayShot()
{
    shot.play(0);
}

void SoundFX::StartUfo()
{
    if (ufoChannel == SOUND_CH_UNDEF)
        ufoChannel = ufo.play(-1);
}

void SoundFX::StopUfo()
{
    if (ufoChannel != SOUND_CH_UNDEF)
        ufo.halt(ufoChannel);

    ufoChannel = SOUND_CH_UNDEF;
}

void SoundFX::PlayUfoHit()
{
    ufoHit.play(0);
}

void SoundFX::PlayWalk1()
{
    walk1.play(0);
}

void SoundFX::PlayWalk2()
{
    walk2.play(0);
}

void SoundFX::PlayWalk3()
{
    walk3.play(0);
}

void SoundFX::PlayWalk4()
{
    walk4.play(0);
}

void SoundFX::PlayBeginPlay()
{
    beginPlay.play(0);
}

void SoundFX::PlayExtraLife()
{
    extraLife.play(0);
}

void SoundFX::free()
{
    coin.free();
    baseHit.free();
    invHit.free();
    shot.free();
    ufo.free();
    ufoHit.free();
    walk1.free();
    walk2.free();
    walk3.free();
    walk4.free();
    beginPlay.free();
    extraLife.free();
}

