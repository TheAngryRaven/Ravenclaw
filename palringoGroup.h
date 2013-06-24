/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

/*	Class: palringoGroup
 *	handles group information gathering
 */

#ifndef PALRINGOGROUP_H
#define PALRINGOGROUP_H

//project includes
#include "misc.h"
#include "crypt.h"
#include "baseClient.h"
#include "palringoPacket.h"
#include "palringoConnection.h"
#include "palringoMessage.h"
#include "palringoContact.h"

class misc;
class crypt;
class baseClient;
class palringoPacket;
class palringoConnection;
class palringoMessage;
class palringoContact;
class palringoGroup
{
	public:
		palringoGroup(palringoConnection *client, baseClient *base);

		//when someone joins or leaves
		void group_update(packet input);

		//when someone does any kind of admin action
		void group_admin(packet data);

		//group join/leave
		void group_join(string groupName, string Password);
		void group_part(string groupID);

		//admin action
		void admin(string action, string groupID, string user);

	protected:
	private:
        map<string, string> parseUpdate(string hexPayload);

	    palringoPacket		palPack;
		palringoConnection	*palConn;
		baseClient			*botClient;

		misc				engine;
		crypt				cipher;
};

#endif // PALRINGOGROUP_H
