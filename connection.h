/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

/*	Class: connection
 *	Creates and handles the base TCP/IP connection
 */

#ifndef CONNECTION_H
#define CONNECTION_H

//library includes
#include <winsock.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdio.h>

//function includes
#include "misc.h"
#include "crypt.h"
#include "palringoConnection.h"

class misc;
class crypt;
class palringoConnection;
class connection
{
	public:
		connection(palringoConnection *client);
		bool	connectToHost(string IP, int PORT);
		void	disconnect(void);

		DWORD	read(void);			//reads from socket
		void	write(string data);	//writes to socket
		void	writeBytes(char* data);	//writes to socket

		bool	connected;

	protected:
	private:
		misc	engine;
		crypt	cipher;
		SOCKET	sock;

		palringoConnection *palConn;
};

#endif // CONNECTION_H
