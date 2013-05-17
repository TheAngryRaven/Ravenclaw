/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

/*	Class: palringoConnection
 *	higher level networking module, handles completed packets and such
 */

#ifndef PALRINGOCONNECTION_H
#define PALRINGOCONNECTION_H

//library includes
#include <algorithm>
#include <vector>

//project includes
#include "misc.h"
#include "crypt.h"
#include "baseClient.h"
#include "connection.h"
#include "palringoClient.h"
#include "palringoPacket.h"
#include "palringoMessage.h"

class misc;
class crypt;
class baseClient;
class connection;
class palringoClient;
class palringoPacket;
class palringoMessage;
class palringoConnection
{
	public:
		palringoConnection(palringoClient *client);

		bool connect(void);		//connect's to palringo server
		void disconnect(void);	//disconnect from server

		void recv_packet(string data);	//we received a packet
		void send_packet(packet data);	//we are sending a packet
		void send_ping();	            //we need to send a ping

		baseClient* get_Client(void); // returns pointer to clientUser

	protected:
	private:
		void send_logon(void);	//we need to send the logon
		void send_auth(packet data);	//we need to send the auth

		void parse_recv(string data);
		void parse_packet(packet data);

		palringoClient	*palClient;
		palringoPacket	palPack;
		palringoMessage	*palMsg;

		baseClient		*clientUser;
		connection		*conn;
		misc			engine;
		crypt			cipher;
		bool 			loggedIn;
		int             pingsSent;

};

#endif // PALRINGOCONNECTION_H
