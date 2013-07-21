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
                        "password=123456\r\n"
                        "REQUIRED it's used in one or two applications currently to prevent the bot 'talking to itself'\r\n"
                        "botId=123456\r\n"
                        "#SSL yes or no\r\n"
                        "HTTPS=no\r\n\r\n"
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
                        "#Required for an offical palringo bot turns [#help] into [#rc help]\r\n"
                        "#Namespace can be anything as long as it has no spaces\r\n"
                        "nameSpace=rc\r\n"
                        "#if you delete the namespace field the bot will work as normal ie: '#help'\r\n"
                        "#Please see readme.html for more ini settings"
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
        map<string, string> botSettings;

        //get LOGON details
        botSettings["username"]     = reader.Get("LOGON", "email", "UNKNOWN");
        botSettings["password"]     = reader.Get("LOGON", "password", "UNKNOWN");
        botSettings["botId"]        = reader.Get("LOGON", "botId", "UNKNOWN");
        botSettings["HTTPS"]        = reader.Get("LOGON", "SSL", "true");

        //get bot settings
        botSettings["botAdmin"]     = reader.Get("SETTINGS", "adminId", "UNKNOWN");
        botSettings["adminName"]    = reader.Get("SETTINGS", "adminName", "UNKNOWN");
        botSettings["botName"]      = reader.Get("SETTINGS", "botName", "UNKNOWN");
        botSettings["cmdAdmin"]     = reader.Get("SETTINGS", "cmdAdmin", "#");
        botSettings["cmdUser"]      = reader.Get("SETTINGS", "cmdUser", "/");
        botSettings["nameSpace"]    = reader.Get("SETTINGS", "nameSpace", "UNKNOWN");

		#ifdef RAVENCLAW_DEBUG
        //obtain user script settings
        botSettings["useScripts"]	= reader.Get("WEBSCRIPTS", "useWebScripts", "false");
        botSettings["postUrl"]		= reader.Get("WEBSCRIPTS", "postUrl", "UNKNOWN");
        #endif

        //Get the location of the bots ini file
        botSettings["iniFile"]      = iniFile;


        if(botSettings["username"]      == "UNKNOWN" ||
           botSettings["password"]      == "UNKNOWN" ||
           botSettings["botAdmin"]      == "UNKNOWN" ||
           botSettings["botName"]       == "UNKNOWN" ||
           botSettings["adminName"]     == "UNKNOWN" ||
           botSettings["botId"]     == "UNKNOWN" )
        {
            engine.pl("Bot cannot find required field(s) for logging on\r\nPlease check your .ini file");
            engine.pause();
            return 1;
        }
        #ifdef RAVENCLAW_DEBUG
        else if(botSettings["useScripts"] != "false" && botSettings["postUrl"] == "UNKNOWN")
		{
			engine.pl("You have enabled webScripts but have not supplied a URL please fix this issue.");
            engine.pause();
            return 1;
		}
		#endif

        spinUp(botSettings["botName"]);
        spinning(botSettings);
        spinDown();

        return 0;
    }
}

void spinUp(string botName)
{
    string titleName = "PalringoBot: "+botName;
	SetConsoleTitle( titleName.c_str() );
}

void spinning(map<string, string> botSettings)
{
	engine.pl("Main-> starting palClient", 1);
	palClient = new palringoClient(botSettings);
	palClient->run();
}

void spinDown()
{
	delete palClient;
	engine.pause();
}
