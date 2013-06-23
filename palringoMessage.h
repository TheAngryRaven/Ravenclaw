/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

/*	Class: palringoMessage
 *	module for sending and receiving messages
 */

#ifndef PALRINGOMESSAGE_H
#define PALRINGOMESSAGE_H

//project includes
#include "misc.h"
#include "crypt.h"
#include "baseClient.h"
#include "palringoPacket.h"
#include "palringoConnection.h"
#include <algorithm>
#include <vector>
#include <ctime>

class misc;
class crypt;
class baseClient;
class palringoPacket;
class palringoConnection;
class palringoMessage
{
	public:
		palringoMessage(palringoConnection *client, baseClient *base);

		//message handleing
		void recv_message(packet data);
		void send_message(string target, string to, string message);
		void send_image(string target, string group, string imgPath);
		void send_debug(string target, string to);

	protected:

	private:
		palringoPacket		palPack;
		palringoConnection	*palConn;
		baseClient			*botClient;
		misc				engine;
		crypt				cipher;

};

#endif // PALRINGOMESSAGE_H
