/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

#include "palringoClient.h"

palringoClient::palringoClient(string username, string password, string botAdmin, string adminName, string botName, string cmdAdmin, string cmdUser)
{
	clientUser	= new baseClient(username, password, botAdmin, adminName, botName, cmdAdmin, cmdUser);
	palConn		= new palringoConnection(this);
}

void palringoClient::run()
{
	engine.pl("palClient-> Attempting connection");
	bool connected = palConn->connect();

	if(connected)
	{
		engine.pl("palClient-> We are connected");
		//engine.pl("palClient-> We connected, now disconnecting");
		//palConn->disconnect();
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
