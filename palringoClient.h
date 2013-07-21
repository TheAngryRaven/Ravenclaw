/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

/*	Class: palringoClient
 *	Initializes all palringo functions, from the client to the connector
 */

#ifndef PALRINGOCLIENT_H
#define PALRINGOCLIENT_H

//project includes
#include "misc.h"
#include "baseClient.h"
#include "palringoConnection.h"

class baseClient;
class palringoConnection;
class palringoClient
{
	public:
        //Yay Command like arguments
		palringoClient(map<string, string> botSettings);
		void run();		//starts and creates everything
		baseClient* get_Client(void); // returns pointer to clientUser

	protected:
	private:
		misc engine;
		baseClient			*clientUser;
		palringoConnection	*palConn;
};

#endif // PALRINGOCLIENT_H
