/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

/*	Class: packet
 *	Used for creating bits of data to send over the network
 */

#ifndef PACKET_H
#define PACKET_H

//library includes
#include <iostream>
#include <sstream>
#include <cstring>

//project includes
#include "misc.h"
#include "crypt.h"

class misc;
class crypt;
struct packetHeader
{
	string key;
	string value;
};
class packet
{
	public:
		packet();
		string serialize(void);	//formulates the packet into one character array

		void addHeader(string name, string data);
										//gives our packet a standard header

		void addCommand(string data);	//gives our packet a command header
		void addPayload(string data);	//adds a payload onto the packet

		string search_headers(string query);


        string          NULLHEADER = "no-header";
		string			getCommand();
		string			getPayload();
		packetHeader	getHeader(int number);
		int				getHeaders();

	protected:
	private:
		misc	engine;
		crypt	cipher;
		string	command;
		string	payload;
		char*	cPayload;
		string	headers[32][2];
		int		headCnt;
};

#endif // PACKET_H
