/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

/*	Class: baseClient
 *	Handles messages to and from palringo, with chatting, administration, etc etc
 */

#ifndef BASECLIENT_H
#define BASECLIENT_H

//library includes
#include <iostream>
#include <string.h>
#include <algorithm>
#include <vector>

//project includes
#include "misc.h"
#include "crypt.h"
#include "palringoMessage.h"

using namespace std;

class misc;
class crypt;
class palringoMessage;
class baseClient
{
	public:
		baseClient();

		//its wise not to "run" these functions
		//these get called by other various application functions
		void recv_message(string group, string user, string message);
		void recv_pm(string name, string user, string message);

		//these functions should not be touched
		void	set_palMesg(palringoMessage *mesg);	//sets the palringoMessage pointer
		string 	get_Username(void);	//returns username
		string 	get_Password(void);	//returns password

	protected:
		//Functions to send things
		void send_message(string group, string message);
		void send_pm(string id, string message);
		void send_image(void);

		//Group join and leave functions
		void group_join(string groupName, string password = "");
		void group_part(string groupID);
		void group_update(void);

		//Self Explaining admin actions
		void admin_admin(string groupID, string userID);
		void admin_mod(string groupID, string userID);
		void admin_silence(string groupID, string userID);
		void admin_reset(string groupID, string userID);
		void admin_kick(string groupID, string userID);
		void admin_ban(string groupID, string userID);

		//"working" functions
		void   parse_commands(string group, string user, vector<string> data);
		string messagePatcher(vector<string> message, string patch = " ", int start = 1);

	private:
		string	username, password, botAdmin, botName, adminName, controlGroup;
		string  cmdAdmin, cmdBase;
		bool	canTalk;
		misc	engine;
		crypt	cipher;

		palringoMessage 	*palMesg;

};

#endif // BASECLIENT_H
