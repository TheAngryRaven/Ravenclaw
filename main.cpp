/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

/* Credits
 * Ross (for the original php framework)
 * Folks on palringo (for new auth procedure)
 * Current palringo staff members are douchebags
 * Nom helped alot and so did sniper/caelean
 */

#include "main.h"

int main()
{
    spinUp();
    spinning();
    spinDown();

    return 0;
}

void spinUp()
{
	SetConsoleTitle( "PalringoBot: Crimson" );
}

void spinning()
{
	engine.pl("Main-> starting palClient", 1);
	palClient->run();
}

void spinDown()
{
	delete palClient;
	engine.pause();
}
