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

int main(int argc, char* argv[])
{
    if (argc < 5)
    {
        //get base filename not full argv[0] path
        char *filePath = strrchr(argv[0], '\\');
        string trueName = filePath;
        trueName.erase(0, 1);

        cerr << "Bot Cannot start\n"
                "Logon Details are no longer compiled\n\n"
                "Usage: "<< trueName <<" [Email] [Password] [Admin ID] [Admin Name] [Bot Name]" << endl;

        engine.pause();
        return 1;
    }
    else
    {
        string username = argv[1];
        string password = argv[2];
        string botAdmin = argv[3];
        string adminName= argv[4];
        string botName  = argv[5];

        string cmdAdmin = "#";
        string cmdUser = "/";

        if(argv[6])
            cmdAdmin = argv[6];

        if(argv[7])
            cmdUser = argv[7];

        /*
        engine.pl("Username: "+ username +
                  "\nPassword: "+password+
                  "\nAdmin ID: "+ botAdmin+
                  "\nadminName: "+ adminName+
                  "\nbotName: "+ botName+
                  "\ncmdAdmin: "+ cmdAdmin+
                  "\ncmdUser: "+ cmdUser
                  );
        engine.pause();
        */


        spinUp();
        spinning(username, password, botAdmin, adminName, botName, cmdAdmin, cmdUser);
        spinDown();

        return 0;
    }
}

void spinUp()
{
	SetConsoleTitle( "PalringoBot: Crimson" );
}

void spinning(string username, string password, string botAdmin, string adminName, string botName, string cmdAdmin, string cmdUser)
{
	engine.pl("Main-> starting palClient", 1);
	palClient = new palringoClient(username, password, botAdmin, adminName, botName, cmdAdmin, cmdUser);
	palClient->run();
}

void spinDown()
{
	delete palClient;
	engine.pause();
}
