/**
 *  @file    Video.h
 *  @author  Kumar Pillai
 *  @date    01/Jan/2017
 *  @version 1.0
 *
 *  @section DESCRIPTION
 *
 * CPU class header.
 *
 */

#ifndef VIDEO_H
#define VIDEO_H

#include <fstream>
#include <string>
#include <vector>
#include "Core/Graphics.h"
#include "Core/OutlineFont.h"
#include "Memory.h"

const int SPIN_SCREEN_W = 224*2;
const int SPIN_SCREEN_H = 256*2;

class Video
{
private:
    int X;
    int Y;

    Graphics* gVideo;
    OutlineFont videoOutlineFont;
    Memory* videoMemory;
    Byte* videoMemoryPtr;

    // Retro
    int color[32][2];
    int color2[32][2];
    // Retro

    // One pre-positioned piece of text in the right-hand help panel
    struct PanelText
    {
        std::string text;
        int x, y;
        int r, g, b;
        int creditsNeeded;  // > 0 for section titles that can be highlighted as READY
    };

    // Help panel layout, computed once by layoutHelpPanel() and redrawn every frame
    std::vector<PanelText> panelTexts;
    bool panelLaidOut;
    int panelX, panelRight;
    unsigned int panelFrame;  // frame counter for the blinking READY marker

    void layoutHelpPanel();
    void addPanelText(const std::string& text, int x, int y, int r, int g, int b, int creditsNeeded = 0);
    void getGameState(int& credits, bool& inGame);
    int textWidth(const std::string& text);
    int lineHeight();
    std::vector<std::string> wrapText(const std::string& text, int maxWidth);
public:
    Video();
    ~Video();

    bool init(Graphics* g, Memory* m);
    void draw();
    void drawHelpPanel();
    void drawOutlineFont(const char text[], int x, int y, int r, int g, int b);
    void copyScreen();
    void free();

    OutlineFont* getVideoOutlineFont();
};

#endif

