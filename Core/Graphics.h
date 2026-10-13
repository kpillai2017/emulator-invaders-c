#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL/SDL.h>

class Graphics
{
private:
    SDL_Surface* backbuffer;
    int width;
    int height;
public:
    bool init(int aWidth, int aHeight, bool aFullscreen);
    void drawPixel(int x, int y, int r, int g, int b);
    void drawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b);
    void drawRect(int x, int y, int width, int height, int r, int g, int b);
    void fillRect(int x, int y, int width, int height, int r, int g, int b);
    void clear(int r, int g, int b);
    void flip();
    int getWidth();
    int getHeight();
    SDL_Surface* getBackbuffer();
};

#endif
