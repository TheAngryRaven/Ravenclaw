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
#include <ctime>
#include <time.h>

//project includes
#include "misc.h"
#include "crypt.h"
#include "curl.h"
#include "regex.h"
#include "palringoMessage.h"
#include "palringoGroup.h"
#include "palringoContact.h"
#include "palringoDatabase.h"
#include "definitions.h"

struct botMod{
    string id;
    string name;
    string message = "";
    bool online = false;
};

class misc;
class crypt;
class curl;
class regex;
class palringoMessage;
class palringoGroup;
class palringoContact;
class palringoDatabase;
class baseClient
{
	public:
		baseClient(map<string, string> botSettings);

		//The following 4 functions are called from palringoConnection when needed
		void recv_groupMessage(string group, string user, string message);
		void recv_personalMessage(string name, string user, string message);

		//group update and admin actions also dont touch
		void group_update(map<string, string> updatePacket);
		void group_admin(string group, string admin, string user, string action);

		//these functions should not be touched
		void	set_palMesg(palringoMessage *mesg);	//sets the palringoMessage pointer
		void	set_palGroup(palringoGroup *group);	//sets the palringoGroup pointer
		void	set_palContact(palringoContact *contact);	//sets the palringoContact pointer

        //returns pointers to componets
		palringoContact	*get_palContact();	//returns pointer to palContact

		string 	get_Username(void);	//returns username
		string 	get_Password(void);	//returns password

		//beta features
		void    parseResponse(packet response, packet input);

	protected:
		//Functions to send things
		void send_message(string group, string message);
		void send_pm(string id, string message);
		void send_image(string group, string image);
		void send_image_pm(string user, string image);

		//packet used for testing random things
		void send_debug(string to);

		//Group join and leave functions
		void group_join(string groupName, string password = "");
		void group_part(string groupID);

		//Self Explaining admin actions
		void admin_admin(string groupID, string userID);
		void admin_mod(string groupID, string userID);
		void admin_silence(string groupID, string userID);
		void admin_reset(string groupID, string userID);
		void admin_kick(string groupID, string userID);
		void admin_ban(string groupID, string userID);

		//"working" functions
		void    parse_groupMessage(string group, string user, vector<string> data);
		void    parse_personalMessage(string name, string user, vector<string> data);
		string  messagePatcher(vector<string> message, string patch = " ", int start = 2);

		void    reloadIni(string group = "");

	private:
		string	username, password, botAdmin, botName, adminName, adminMessage, botId;
		string  cmdAdmin, cmdBase, nameSpace;
		bool	canTalk, adminOnline, security;
		time_t  startTime;
		misc	engine;
		crypt	cipher;
		regex	Regex;

		curl    Curl;
		//palringoDatabase palDB;

		palringoMessage *palMesg;
		palringoGroup 	*palGroup;
		palringoContact *palContact;

		string iniFile;
		int patchStart;

		map<string, botMod> botMods;
		bool isMod(string userid);
		string searchMod(string modname);

		vector<string> botPests;
		bool isPest(string userid);

		vector<string> muteList;
		bool shouldMute(string userid);

		vector<string> banList;
		bool shouldBan(string userid);

		//Beta Features
		unsigned long   messagesReceived;
		unsigned long   messagesSent;
		bool            debug;
};

#endif // BASECLIENT_H
