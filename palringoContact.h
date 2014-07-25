/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

/*	Class: palringoContact
 *	handles server and client side information gathering
 */

#ifndef PALRINGOCONTACT_H
#define PALRINGOCONTACT_H

//library includes
#include <iostream>
#include <string.h>
#include <algorithm>
#include <vector>

//project includes
#include "misc.h"
#include "crypt.h"
#include "baseClient.h"
#include "palringoPacket.h"
#include "palringoConnection.h"
#include "palringoMessage.h"
#include "palringoContact.h"

struct contact{
    string nickname;
	string userId;
    string status;
	string privileges;
	string rep;
	string icon;
	string onlineStatus;
    string device;
};

//TODO: Sub Profile Parsing
class misc;
class crypt;
class baseClient;
class palringoPacket;
class palringoConnection;
class palringoMessage;
class palringoContact
{
	public:
		palringoContact(palringoConnection *client, baseClient *base);

		//client side actions
		void    client_add(map<string, string> data);   //adds user details to struct map
		contact client_lookup(string id);               //looks up user details based on ID

        //server side actions
		void server_request(void);	//Someone added you to their contact list

		void server_add(void);		//add someone to your contacts list by using their id
		void server_accept(void);	//accept someones contact request
		void server_decline(void);	//decline someones contact request
		void server_block(void);	//block a user from messaging you

		void parse_subprofile(packet profilePacket);

		wchar_t *CodePageToUnicode(int codePage, const char *src);
		char *UnicodeToCodePage(int codePage, const wchar_t *src);

	protected:
	private:
        misc	engine;
		crypt	cipher;

		palringoConnection	*palConn;
		baseClient			*botClient;

	    map<string, contact> contactList;
};

#endif // PALRINGOCONTACT_H
