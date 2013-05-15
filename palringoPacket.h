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
#include <windows.h>
using namespace std;


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

		packet logon(string email);		//creates simplistic logon packet

		packet auth(string password,	//creates auth packet with generated password
					packet data);

		packet ping(void);				//creates simplistic ping packet

		packet message(	string target,	//creates a packet with a message to send
						string to,
						string payload);

		packet admin(	string action,	//creates a admin packet (admin, mod, etc)
						string group, string target);


		packet image(	string target,	//creates an image packet
						string to,
						char* payload);

		packet imageHeader(string target, string to, char* payload, int size);

		packet group_join(string group, string password = "");		//creates a packet to join a group
		packet group_part(string group);		//creates a packet to leave a group


	protected:
	private:
		misc	engine;
		crypt	cipher;
		string	generate_payload(string password, packet data);
};

#endif // PALRINGOPACKET_H
