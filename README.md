# Space Invaders Emulator (C++)

Game ROMs are not included. Supply your own legally obtained ROM as described below.  

### The Intel 8080 emulator for the Taito Space Invaders ###
The game engine is modified from *The Black Art of Multiplatform Game Programming* by Jazon Yamamoto, modified to run this emulator. The emulator (written in C++) is based on Remi Veilleux's implementation on [BlitzMax Basic](https://sites.google.com/site/rveilleux/blitzmaxarcadeemulatortutorial).  

### How to play ###

**Starting a game**

| Mode | Steps |
|---|---|
| 1 player | Press `3` (or `5`) once to insert a coin, then press `1`. |
| 2 players | Press `3` (or `5`) twice to insert 2 coins (CREDIT shows `02`), then press `2`. |

In 2-player mode the players take turns, as on the original cabinet: Player 2 plays when Player 1 loses a life. Both players use the same keys.

**Controls**

| Action | Key |
|---|---|
| Insert coin | `3` or `5` |
| 1 player start | `1` |
| 2 player start | `2` |
| Move left | `Left Arrow` |
| Move right | `Right Arrow` |
| Fire | `Space` |
| Tilt | `T` |
| Quit | `Escape` |

The help panel on the right of the window shows these steps. When enough credits are inserted (and no game is running) the matching section is highlighted with a blinking `<< READY` marker.

### Recent changes ###

**On-screen help panel** (`Video.cpp`, `Video.h`)  
- The banner now reads "Space Invaders emulator started."  
- A panel to the right of the game explains 1- and 2-player mode step by step and lists the controls.  
- The panel content is defined in one table, `HELP_ROWS` in `Video.cpp`. Text wraps to the panel width, and numbered steps get a hanging indent.  
- The layout is worked out once, on the first frame, and then reused.  
- When enough credits are inserted, the matching section is highlighted with a blinking `<< READY` marker. The credit count and game-in-progress flag are read from the game's RAM (`0x20EB` and `0x20EF`).  
- The layout is identical to the Python version's panel.  

**Text measurement** (`Core/OutlineFont.cpp`, `Core/OutlineFont.h`)  
- Added `textWidth()` (`TTF_SizeText`) and `lineHeight()` (`TTF_FontLineSkip`).  
- `draw()` now takes `const char*`, and skips empty text and failed renders.  

**CPU** (`Cpu.cpp`, `Cpu.h`)  
- Added the `SBB M` instruction (opcode `0x9E`), which was missing. The Python version already had it.  

All new code is C++98-compatible, because the Code::Blocks project does not set a `-std` flag.

### How do I get set up? ###

**Game ROM (not included)**  
Obtain ROMs you are legally entitled to use and place the combined 8192-byte ROM at `data/invaders.rom`. Combine the four 2048-byte files in the order `invaders.h`, `invaders.g`, `invaders.f`, `invaders.e`; see [data/README.md](data/README.md). ROM files and backup/test variants are ignored by Git. Do not force-add them or include them in distributed builds. Sound effects and the font remain included.

**Summary of set up**  
Development environment is [Code::Blocks](http://www.codeblocks.org). Download the Windows binary release that comes with MingW (MingW stands for Minimalist GNU for Windows). In reality, MingW is the compiler and Code::Blocks is just an IDE (Integrated Development Environment) that you can use with different compilers. If you download the version that does not come with MingW, you will have to install MingW manually. To avoid complications, just make sure you get the version that comes packaged with MingW.    

**Setting Up SDL**  
Download the [SDL libraries](https://www.libsdl.org). You will need the *SDL 1.2 Development* library, that are configured for MingW. Make sure the correct version (32- or 64-bit) is used. Once downloaded, create a folder on the C drive of your computer (or any drive that is convenient) and name it *SDL*.    

Download extensions of SDL: [images](http://www.libsdl.org/projects/SDL_image/release-1.2.html), [sound](http://www.libsdl.org/projects/SDL_mixer/release-1.2.html), and [font ](http://www.libsdl.org/projects/SDL_ttf/release-1.2.html). The names of the files are **SDL_image-devel-1.2.12-VC.zip**,
**SDL_mixer-devel-1.2.12-VC.zip**, and **SDL_ttf-devel-2.0.11-VC.zip**.  

After extracting each of those files, they should each contain a folder called *include* and a folder called *lib*. Copy the files in the lib folder that have a *.lib* extension and paste them in the *lib* folder in your SDL directory. Next, copy the files in the *include* directory and paste them in the folder called *SDL*, which is located in the include folder of your *SDL* directory. There are also DLL files in the lib folder, and these are going to be necessary later on, so do not lose them.    

**Configuring Code::Blocks**  
Open Code::Blocks and click on *Settings → Compiler and Debugger*. Then, click on *Global Compiler Settings → Linker Settings*. In the *Other Linker Options* box, type **-lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer**. This list doesn’t have to be on the same line; each item can be on a separate line. After that, go to the *Search Directories* tab. Under the *Compiler* tab, click on *Add* and add the SDL include directory. You can also use the *Browse* button to locate the folder. Now go to the *Linker* tab and add *lib* folder location.    

**Dependencies**  
*libfreetype-6.dll*  
*libogg-0.dll*  
*libvorbis-0.dll*  
*libvorbisfile-3.dll*  
*mikmod.dll*  
*SDL.dll*  
*SDL_image.dll*  
*SDL_mixer.dll*  
*SDL_ttf.dll*  
*smpeg.dll*  
*zlib1.dll*  

**Running**  
Run the executable from the project folder, the one containing `data/` and `fonts/`, because the ROM (`data/invaders.rom`), sound effects (`data/*.wav`) and font (`fonts/blitz.fon`) are loaded by relative path. Copy the DLLs listed above next to the executable.  
