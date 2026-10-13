#include "Io.h"

Io::Io()
{

}

Io::~Io()
{

}

void Io::init(Input* in, SoundFX* sFX)
{
    IoSoundFX = sFX;
    IoInput = in;

    // Dipswitch: BIT0 and BIT1 controls starting number of life (from 3 to 6)
    // BIT7 prints additionnal coin message on intro screen
    IN_PORT2 = 0;
    IN_PORT2 |= (BIT0 | BIT1);
    IN_PORT2 |= (BIT7);
}

//*********************************************************************
void Io::update()
{
    // Clear player input bits
    IN_PORT1 = IN_PORT1 & (~(BIT0 | BIT1 | BIT2 | BIT4 | BIT5 | BIT6));
    IN_PORT2 = IN_PORT2 & (~(BIT2 | BIT4 | BIT5 | BIT6));

    if(IoInput->keyDown(SDLK_1)) IN_PORT1 |= BIT2; // Player 1 start
    if(IoInput->keyDown(SDLK_2)) IN_PORT1 |= BIT1; // Player 2 start
    if(IoInput->keyDown(SDLK_t)) IN_PORT2 |= BIT2; // Tilt detection trigger

	// Move left: same key used for player 1 and 2
    if(IoInput->keyDown(SDLK_LEFT))
	{
		IN_PORT1 |= BIT5;
		IN_PORT2 |= BIT5;
	}

	// Move right: same key used for player 1 and 2
    if(IoInput->keyDown(SDLK_RIGHT))
	{
		IN_PORT1 |= BIT6;
		IN_PORT2 |= BIT6;
	}

	// Shoot button: same key used for player 1 and 2
    if(IoInput->keyDown(SDLK_SPACE))
	{
		IN_PORT1 |= BIT4;
		IN_PORT2 |= BIT4;
	}

	// Insert coin: we play a sound effect here just for fun:
	// this sound is NOT produced by the machine
	if (IoInput->keyDown(SDLK_3) or IoInput->keyDown(SDLK_5))
	{
		IN_PORT1 |= BIT0;
		IoSoundFX->PlayCoin();
	}
}

void Io::outputPort(Byte port, Byte value)
{
    switch(port)
    {
        // Port 2 simply stores a 8-bit value
        case 2:
            OUT_PORT2 = value;
            break;

        // Port 3 controls play of various sound effects
        case 3:
            if(value & BIT0)
                IoSoundFX->StartUfo();
            else
                IoSoundFX->StopUfo();

            if((value & BIT1) and not (OUT_PORT3 & BIT1)) IoSoundFX->PlayShot();
            if((value & BIT2) and not (OUT_PORT3 & BIT2)) IoSoundFX->PlayBaseHit();
            if((value & BIT3) and not (OUT_PORT3 & BIT3)) IoSoundFX->PlayInvHit();
            if((value & BIT4) and not (OUT_PORT3 & BIT4)) IoSoundFX->PlayExtraLife();
            if((value & BIT5) and not (OUT_PORT3 & BIT5)) IoSoundFX->PlayBeginPlay();
            OUT_PORT3 = value;
            break;

        // Port 4 stores a 16-bit value (by storing two 8-bit values)
        case 4:
            OUT_PORT4LO = OUT_PORT4HI;
            OUT_PORT4HI = value;
			break;

        // Port 5 also controls sound
        case 5:
            if((value & BIT0) and not (OUT_PORT5 & BIT0)) IoSoundFX->PlayWalk1();
            if((value & BIT1) and not (OUT_PORT5 & BIT1)) IoSoundFX->PlayWalk2();
            if((value & BIT2) and not (OUT_PORT5 & BIT2)) IoSoundFX->PlayWalk3();
            if((value & BIT3) and not (OUT_PORT5 & BIT3)) IoSoundFX->PlayWalk4();
            if((value & BIT4) and not (OUT_PORT5 & BIT4)) IoSoundFX->PlayUfoHit();
            OUT_PORT5 = value;
            break;
    }
}

Byte Io::inputPort(Byte port)
{
    Byte result;

    switch(port)
    {
        // Player 1 Input keys:
        case 1:
            result = IN_PORT1;
            break;

        // Player 2 Input keys And dip switches:
        case 2:
            result = IN_PORT2;
            break;

        // Port 3 calculate using values stored in output port 2 and 4
        case 3:
            result = (((OUT_PORT4HI << 8) | OUT_PORT4LO) << OUT_PORT2) >> 8;
            break;
    }

    return result;
}
