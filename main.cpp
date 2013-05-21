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
    if (argc != 2)
    {
        //get base filename not full argv[0] path
        char *filePath = strrchr(argv[0], '\\');
        string trueName = filePath;
        trueName.erase(0, 1);

        cout << "Bot Cannot start\n"
                "Logon details are loaded from an ini file\n\n"
                "Usage: "<< trueName <<" [file.ini]\r\n"
                "An example file has been created for you\r\nAlong with an example batch script to start the bot properly" << endl;

        string iniFile  = "example.ini";
        string batch    = "runBot.bat";

        //deletes preexisting example file
        remove(iniFile.c_str());
        remove(batch.c_str());

        //creates a super basic ini filee
        ofstream iniStream;
        iniStream.open(iniFile.c_str(), ios::out | ios::app | ios::binary);
        iniStream <<    "[LOGON]\r\n"
                        "#your palringo logon details\r\n"
                        "email=example@email.com\r\n"
                        "password=123456\r\n\r\n"
                        "[SETTINGS]\r\n"
                        "#the user ID of the user in chage of the bot\r\n"
                        "adminId=1234\r\n\n"
                        "#A plain text name for the admin\r\n"
                        "adminName=myName\r\n\n"
                        "#A plain text name for the bot to respond tor\n"
                        "botName=ravenclaw\r\n\n"
                        "#the prefix for admin bot commands ie: #join or #leave\r\n"
                        "cmdAdmin=#\r\n\n"
                        "#the prefix for user bot commands ie: /help or /google\r\n"
                        "cmdUser=/\r\n"
                        ;
        iniStream.close();

        //creates the batch script
        iniStream.open(batch.c_str(), ios::out | ios::app | ios::binary);
        iniStream << trueName << " example.ini";
        iniStream.close();

        engine.pause();
        return 1;
    }
    else if(argc == 2)
    {
        string iniFile = argv[1];
        INIReader reader = INIReader(iniFile);

        if (reader.ParseError() < 0)
        {
            cout << "Can't load " << iniFile << "\n";
            return 1;
        }
        //get LOGON details
        string username     = reader.Get("LOGON", "email", "UNKNOWN");
        string password     = reader.Get("LOGON", "password", "UNKNOWN");

        //get bot settings
        string botAdmin     = reader.Get("SETTINGS", "adminId", "UNKNOWN");
        string adminName    = reader.Get("SETTINGS", "adminName", "UNKNOWN");
        string botName      = reader.Get("SETTINGS", "botName", "UNKNOWN");
        string cmdAdmin     = reader.Get("SETTINGS", "cmdAdmin", "#");
        string cmdUser      = reader.Get("SETTINGS", "cmdUser", "/");

        if(username     == "UNKNOWN" ||
           password     == "UNKNOWN" ||
           botAdmin     == "UNKNOWN" ||
           adminName    == "UNKNOWN" ||
           botName      == "UNKNOWN" )
        {
            engine.pl("Bot cannot find required field(s) for logging on\r\nPlease check your .ini file");
            engine.pause();
            return 1;
        }

        spinUp(botName);
        spinning(username, password, botAdmin, adminName, botName, cmdAdmin, cmdUser);
        spinDown();

        return 0;
    }
    /*
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


        spinUp(botName);
        spinning(username, password, botAdmin, adminName, botName, cmdAdmin, cmdUser);
        spinDown();

        return 0;
    }
    */
}

void spinUp(string botName)
{
    string titleName = "PalringoBot: "+botName;
	SetConsoleTitle( titleName.c_str() );
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
