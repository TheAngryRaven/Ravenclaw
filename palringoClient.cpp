/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

#include "palringoClient.h"

palringoClient::palringoClient(map<string, string> botSettings)
{
    bool SSL = false;
	if( botSettings["SSL"] == "true" ||
        botSettings["SSL"] == "TRUE" ||
        botSettings["SSL"] == "yes" ||
        botSettings["SSL"] == "YES")
    {
        SSL = true;
    }

    clientUser	= new baseClient(botSettings);
	palConn		= new palringoConnection(this, SSL);
}

void palringoClient::run()
{
	engine.pl("palClient-> Attempting connection");
	bool connected = palConn->connect();

	if(connected)
	{
		engine.pl("palClient-> We are connected");
		while(connected)
		{
			Sleep(1000);
		}

	}
	else
	{
		engine.pl("palClient-> Could not connect");
	}
}

baseClient* palringoClient::get_Client() { return this->clientUser; }
