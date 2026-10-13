#ifndef GAME_H
#define GAME_H

#include <SDL/SDL_ttf.h>

#include "Graphics.h"
#include "Input.h"
#include "Audio.h"
#include "OutlineFont.h"

class Game
{
private:
    Graphics graphics;
    Input input;
    Audio audio;
    OutlineFont outlineFont;
    int fps;
    bool isDone;
public:
    Game();
    ~Game();

    unsigned int getTicks();
    void setFPS(int f);
    void delay(int ticks);
    bool initSystem(char title[], int width, int height, bool fullscreen);
    void freeSystem();
    void run();
    void end();

    virtual bool init();
    virtual void free();
    virtual void update();
    virtual void draw(Graphics* g);

    Graphics* getGraphics();
    Input* getInput();
    Audio* getAudio();
    OutlineFont* getOutlineFont();
};

#endif
