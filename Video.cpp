/**
 *  @file    Video.cpp
 *  @author  Kumar Pillai
 *  @date    01/Jan/2017
 *  @version 1.0
 *
 *  @section DESCRIPTION
 *
 *  Initialises and renders the video.
 *
 */

#include "Video.h"
#include <sstream>
#include <cctype>

namespace
{
    // Right-hand help panel content. Keep keys in sync with Io.cpp.
    //   HELP_TITLE : section heading
    //   HELP_TEXT  : sentence, word-wrapped to the panel width; a leading
    //                "1. " style number gets a hanging indent
    //   HELP_KEY   : (action, key) row of the aligned controls table
    // creditsNeeded (titles only): the section is highlighted as READY when no
    // game is in progress and at least this many credits are inserted (0 = never).
    enum HelpRowType { HELP_TITLE, HELP_TEXT, HELP_KEY };

    struct HelpRow
    {
        HelpRowType type;
        const char* text;
        const char* key;
        int creditsNeeded;
    };

    const HelpRow HELP_ROWS[] =
    {
        { HELP_TITLE, "1 PLAYER GAME", 0, 1 },
        { HELP_TEXT,  "1. Press 3 (or 5) once to insert a coin.", 0 },
        { HELP_TEXT,  "2. Press 1 to start.", 0 },

        { HELP_TITLE, "2 PLAYER GAME", 0, 2 },
        { HELP_TEXT,  "1. Press 3 (or 5) twice to insert 2 coins. CREDIT must show 02.", 0 },
        { HELP_TEXT,  "2. Press 2 to start.", 0 },
        { HELP_TEXT,  "Players take turns: Player 2 plays when Player 1 loses a life.", 0 },
        { HELP_TEXT,  "Both players use the same keys.", 0 },

        { HELP_TITLE, "CONTROLS", 0 },
        { HELP_KEY,   "Move left",  "LEFT arrow" },
        { HELP_KEY,   "Move right", "RIGHT arrow" },
        { HELP_KEY,   "Fire",       "SPACE" },
        { HELP_KEY,   "Tilt",       "T" },
        { HELP_KEY,   "Quit",       "ESCAPE" }
    };

    const int HELP_ROW_COUNT = sizeof(HELP_ROWS) / sizeof(HELP_ROWS[0]);

    // Colours (r, g, b)
    const int TITLE_R = 0,   TITLE_G = 255, TITLE_B = 0;
    const int TEXT_R  = 200, TEXT_G  = 200, TEXT_B  = 200;
    const int KEY_R   = 255, KEY_G   = 255, KEY_B   = 0;
    const int READY_TITLE_R = 255, READY_TITLE_G = 255, READY_TITLE_B = 255;
    const int READY_BAR_R   = 0,   READY_BAR_G   = 110, READY_BAR_B   = 0;
    const char* const READY_SUFFIX = "  << READY";

    // The READY marker blinks: shown for READY_BLINK_FRAMES frames, hidden for
    // the next READY_BLINK_FRAMES (20 frames at 60 FPS = 1.5 blinks per second).
    // The highlight bar and title stay steady.
    const unsigned int READY_BLINK_FRAMES = 20;

    // Space Invaders RAM locations used to drive the panel highlight
    const int RAM_CREDITS   = 0x20EB;  // number of credits, BCD
    const int RAM_GAME_MODE = 0x20EF;  // 0 = attract/demo mode, 1 = game in progress
}

Video::Video()
{
    videoMemoryPtr = NULL;
    gVideo = NULL;
    panelLaidOut = false;
    panelX = 0;
    panelRight = 0;
    panelFrame = 0;
}

Video::~Video()
{
}

/**
 *   @brief  Initialises the video.
 *
 *   @param  g is the Graphics ptr
 *   @param  m is the Memory ptr
 *   @return bool
 */
bool Video::init(Graphics* g, Memory* m)
{
    X = 20;
    Y = 40;
    gVideo = g;
    //pitch = gVideo->getBackbuffer()->pitch >> 2;

    videoMemory = m;
    videoMemoryPtr = m->getMemoryPtr();

    // Retro
    for (int i=0; i<32; i++)
    {
        color[i][0]  = 0x000000; // Black
        color2[i][0] = 0x444444; // Black

        if(i>=26 and i<=27)
        {
            color[i][1]  = 0xFF0000; // Red
            color2[i][1] = 0xAA0000; // Red
        }
        else if(i>=2 and i<=7)
        {
            color[i][1]  = 0x00FF00; // Green
            color2[i][1] = 0x00AA00; // Green
        }
        else
        {
            color[i][1]  = 0xFFFFFF; // White
            color2[i][1] = 0xAAAAAA; // White
        }
    }

    // Retro

    if(!videoOutlineFont.load("fonts/blitz.fon", 14))
        return false;

    // Lay out the help panel lazily on first draw (needs the loaded font)
    panelLaidOut = false;

    return true;
}

/**
 *   @brief  Draw the screen.
 *
 *   @return void
 */
void Video::draw()
{
    gVideo->clear(0, 0, 0);

    drawOutlineFont("Space Invaders emulator started.", 0, 0, 255, 255, 255);

    drawHelpPanel();

    copyScreen();

    // Draw a rectangle around the area where our emulated video output will go
    gVideo->drawLine(X-1, Y-1, X+SPIN_SCREEN_W+1, Y-1, 200, 200, 200);  // Top
    gVideo->drawLine(X-1, Y+SPIN_SCREEN_H, X+SPIN_SCREEN_W, Y+SPIN_SCREEN_H, 200, 200, 200); // Bottom
    gVideo->drawLine(X-1, Y-1, X-1, Y+SPIN_SCREEN_H, 200, 200, 200);  // Left
    gVideo->drawLine(X+SPIN_SCREEN_W, Y-1, X+SPIN_SCREEN_W, Y+SPIN_SCREEN_H, 200, 200, 200);  // Right
}

void Video::drawOutlineFont(const char text[], int x, int y, int r, int g, int b)
{
    videoOutlineFont.draw(text, x, y, r, g, b, gVideo);
}

/**
 *   @brief  Draw the help panel to the right of the emulated screen.
 *
 *   @return void
 */
void Video::drawHelpPanel()
{
    if(!panelLaidOut)
        layoutHelpPanel();

    int credits;
    bool inGame;
    getGameState(credits, inGame);

    const bool showMarker = ((panelFrame / READY_BLINK_FRAMES) % 2) == 0;
    panelFrame++;

    const int barHeight = lineHeight() + 2;
    const int barWidth = panelRight - panelX + 8;

    for (size_t i=0; i<panelTexts.size(); i++)
    {
        const PanelText& t = panelTexts[i];

        if (t.creditsNeeded > 0 && !inGame && credits >= t.creditsNeeded)
        {
            // Section can be started right now: highlight its title
            gVideo->fillRect(t.x - 4, t.y - 1, barWidth, barHeight, READY_BAR_R, READY_BAR_G, READY_BAR_B);
            std::string ready = showMarker ? t.text + READY_SUFFIX : t.text;
            drawOutlineFont(ready.c_str(), t.x, t.y, READY_TITLE_R, READY_TITLE_G, READY_TITLE_B);
        }
        else
        {
            drawOutlineFont(t.text.c_str(), t.x, t.y, t.r, t.g, t.b);
        }
    }
}

/**
 *   @brief  Read the number of credits and whether a game is running from emulated RAM.
 *
 *   @return void
 */
void Video::getGameState(int& credits, bool& inGame)
{
    credits = 0;
    inGame = false;

    if (videoMemoryPtr == NULL)
        return;

    Byte bcd = videoMemoryPtr[RAM_CREDITS];
    credits = (bcd >> 4) * 10 + (bcd & 0x0F);
    inGame = videoMemoryPtr[RAM_GAME_MODE] != 0;
}

/**
 *   @brief  Work out the position of every piece of help panel text (done once).
 *
 *   @return void
 */
void Video::layoutHelpPanel()
{
    panelTexts.clear();

    const int x = X + SPIN_SCREEN_W + 20;
    const int windowWidth = (gVideo != NULL && gVideo->getWidth() > 0) ? gVideo->getWidth() : 800;
    const int right = windowWidth - 10;
    const int indent = 8;
    const int line = lineHeight() + 4;
    panelX = x;
    panelRight = right;

    // Align the key column of the controls table on the widest action label
    int labelWidth = 0;
    for (int i=0; i<HELP_ROW_COUNT; i++)
    {
        if (HELP_ROWS[i].type == HELP_KEY)
        {
            int w = textWidth(HELP_ROWS[i].text);
            if (w > labelWidth) labelWidth = w;
        }
    }
    const int keyX = x + indent + labelWidth + 16;

    int y = Y;
    bool firstSection = true;

    for (int i=0; i<HELP_ROW_COUNT; i++)
    {
        const HelpRow& row = HELP_ROWS[i];
        std::string text(row.text);

        if (row.type == HELP_TITLE)
        {
            if (!firstSection) y += line;  // blank line between sections
            firstSection = false;
            addPanelText(text, x, y, TITLE_R, TITLE_G, TITLE_B, row.creditsNeeded);
            y += line + 2;
        }
        else if (row.type == HELP_KEY)
        {
            addPanelText(text, x + indent, y, TEXT_R, TEXT_G, TEXT_B);
            addPanelText(row.key, keyX, y, KEY_R, KEY_G, KEY_B);
            y += line;
        }
        else
        {
            // Numbered steps ("1. ...") get a hanging indent so wrapped
            // lines line up with the step text, not with the number
            size_t n = 0;
            while (n < text.size() && isdigit((unsigned char)text[n])) n++;

            std::string prefix, body = text;
            if (n > 0 && n + 1 < text.size() && text[n] == '.' && text[n+1] == ' ')
            {
                size_t bodyStart = n + 1;
                while (bodyStart < text.size() && text[bodyStart] == ' ') bodyStart++;
                prefix = text.substr(0, bodyStart);
                body = text.substr(bodyStart);
            }

            const int textX = x + indent + (prefix.empty() ? 0 : textWidth(prefix));
            if (!prefix.empty())
                addPanelText(text.substr(0, n + 1), x + indent, y, TEXT_R, TEXT_G, TEXT_B);

            std::vector<std::string> lines = wrapText(body, right - textX);
            for (size_t l=0; l<lines.size(); l++)
            {
                addPanelText(lines[l], textX, y, TEXT_R, TEXT_G, TEXT_B);
                y += line;
            }
        }
    }

    panelLaidOut = true;
}

void Video::addPanelText(const std::string& text, int x, int y, int r, int g, int b, int creditsNeeded)
{
    PanelText t;
    t.text = text;
    t.x = x;
    t.y = y;
    t.r = r;
    t.g = g;
    t.b = b;
    t.creditsNeeded = creditsNeeded;
    panelTexts.push_back(t);
}

int Video::textWidth(const std::string& text)
{
    int w = videoOutlineFont.textWidth(text.c_str());
    return (w > 0 || text.empty()) ? w : (int)text.size() * 8;  // fallback if font not loaded
}

int Video::lineHeight()
{
    int h = videoOutlineFont.lineHeight();
    return h > 0 ? h : 18;  // fallback if font not loaded
}

/**
 *   @brief  Greedy word-wrap of text into lines no wider than maxWidth pixels.
 *
 *   @return vector of lines
 */
std::vector<std::string> Video::wrapText(const std::string& text, int maxWidth)
{
    std::vector<std::string> lines;
    std::istringstream words(text);
    std::string word, current;

    while (words >> word)
    {
        std::string candidate = current.empty() ? word : current + " " + word;
        if (current.empty() || textWidth(candidate) <= maxWidth)
        {
            current = candidate;
        }
        else
        {
            lines.push_back(current);
            current = word;
        }
    }

    if (!current.empty())
        lines.push_back(current);

    return lines;
}

/**
 *   @brief  Deciphers the emulator screen memory and draws the pixels.
 *
 *   @return void
 */
void Video::copyScreen()
{
    // Cycle thru every byte in memory
    for (int j=0; j<224; j++)
    {
        int src = 0x2400 + (j << 5);
        int r1=0, g1=0, b1=0;
        int r2=0, g2=0, b2=0;
		int k = 0;

        for (int i=0; i<32; i++)
        {
            Byte vram = videoMemoryPtr[src++];

            for (int x=0; x<8; x++)
            {
                // Simulate colour effect(red, green blue); the original consoles had colour transparencies
                // on the screen as rows to give the illusion of colour as the only screen colour was white.
                r1 = ((color [i][vram&1]) & 0xFF0000) >> 16;
                r2 = ((color2[i][vram&1]) & 0xFF0000) >> 16;
                g1 = ((color [i][vram&1]) & 0x00FF00) >> 8;
                g2 = ((color2[i][vram&1]) & 0x00FF00) >> 8;
                b1 = ((color [i][vram&1]) & 0x0000FF);
                b2 = ((color2[i][vram&1]) & 0x0000FF);

                gVideo->drawPixel(X+(j*2), Y+SPIN_SCREEN_H-k-1, r1, g1, b1);  // upright view
 				gVideo->drawPixel(X+(j*2), Y+SPIN_SCREEN_H-k-2, r2, g2, b2);

 				//gVideo->drawPixel(X+SPIN_SCREEN_W-1-(j*2), Y+SPIN_SCREEN_H-k-1, r1, g1, b1);  // upright view
 				//gVideo->drawPixel(X+SPIN_SCREEN_W-1-(j*2), Y+SPIN_SCREEN_H-k-2, r2, g2, b2);  // reversed

				k += 2;
                vram = vram >> 1;
            }
        }
    }
}

/*
/**
 *   @brief  For debugging.
 *
 *   @return void
 */

/*
void Video::copyScreen()
{
    for (int j=0; j<224; j++)
    {
        int src = 0x2400 + (j << 5);
		int k = 0;

        for (int i=0; i<32; i++)
        {
            Byte vram = videoMemoryPtr[src++];

            for (int x=0; x<8; x++)
            {
                int r=0, g=0, b=0;

                if (vram & 0x01)
                {
                    r=0xAA;
                    g=0;
                    b=0;
                }

 				//Video->drawPixel(X+k, Y+j, r, g, b);  // side view
 				gVideo->drawPixel(X+j, Y+SPIN_SCREEN_H-k-1, r, g, b);  // upright view
				k++;
                vram = vram >> 1;
            }
        }
    }
}
*/

/**
 *   @brief  Free video
 *
 *   @return void
 */
void Video::free()
{
    videoOutlineFont.free();
}

/**
 *   @brief  Get the ptr to the outline font object.
 *
 *   @return OutlineFont ptr
 */
OutlineFont* Video::getVideoOutlineFont()
{
    return &videoOutlineFont;
}
