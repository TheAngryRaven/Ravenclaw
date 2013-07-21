/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

/*	Class: palringoPacket
 *	creates premade packets for certain events
 */

#ifndef PALRINGOPACKET_H
#define PALRINGOPACKET_H

//librarys
#include <iostream>
#include <cstring>

//project includes
#include "misc.h"
#include "crypt.h"
#include "packet.h"

class misc;
class crypt;
class packet;
class palringoPacket: public packet
{
	public:
		palringoPacket();

        //auth packets
		packet logon(string email);

		packet auth(string password,
					packet data);

        //tell the servers were still alive
		packet ping(int number);

        //generate the message packet
		packet message(	string target,
						string to,
						string payload);

        //for admin actions
		packet admin(	string action,
						string group, string target);

        //3 required packets for sending an image
		packet imageHeader(string target, string to, string mesgId, string length, string payload);
		packet image(string target, string to, string correlation, string mesgId, string payload);
		packet imageFinal(string target, string to, string correlation, string mesgId, string payload);

        //group subscription packets
		packet group_join(string group, string password = "");
		packet group_part(string group);

		//Test packet, throw random data into it and try to get strange responses from server
		packet debug(string target, string to);

	protected:
	private:
		misc	engine;
		crypt	cipher;
		string	generate_payload(string password, packet data);
};

#endif // PALRINGOPACKET_H
