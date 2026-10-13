#ifndef OUTLINEFONT_H
#define OUTLINEFONT_H

#include <SDL/SDL_ttf.h>
#include "Graphics.h"

class OutlineFont
{
private:
    TTF_Font* font;
public:
    OutlineFont();
    ~OutlineFont();

    bool load(char fileName[], int size);
    void free();

    void draw(const char text[], int x, int y, int r, int g, int b, Graphics* gfx);

    // Text measurement (used to lay out the help panel)
    int textWidth(const char text[]);
    int lineHeight();
};

#endif
