/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

#include "palringoGroup.h"

palringoGroup::palringoGroup(palringoConnection *client, baseClient *base)
{
    palConn		= client;
	botClient	= base;
	botClient->set_palGroup(this);
}

void palringoGroup::group_update(packet input)
{
    map<string, string> buffer = parseUpdate(input.getPayload());
    botClient->group_update( buffer );

    if(buffer["Type"] == "0")
    {
        palringoContact *temp = botClient->get_palContact();
        temp->client_add(buffer);
    }
}

void palringoGroup::group_admin(packet data)
{
    string groupID = data.search_headers("Group-Id");
    string adminID = data.search_headers("Source-Id");
    string userID = data.search_headers("Target-Id");
    string action = data.search_headers("Action");

    botClient->group_admin(groupID, adminID, userID, action);
}

void palringoGroup::group_join(string groupName, string password)
{
	palConn->send_packet(palPack.group_join(groupName, password));
}

void palringoGroup::group_part(string groupID)
{
	palConn->send_packet(palPack.group_part(groupID));
}

void palringoGroup::admin(string action, string groupID, string user)
{
	palConn->send_packet(palPack.admin(action, groupID, user));
}

/////Parsing functions
map<string, string> palringoGroup::parseUpdate(string hexPayload)
{
    //output input
    int         length = hexPayload.length();
    const char* buffer = hexPayload.c_str();


    //output buffer
	map<string, string> updatePacket;

    //Vars for decryptor
	bool    running = true;
	int     iterator = 0;

	int 	start	= 0;
    int 	end		= 0;

    while(running)
    {
	    //temporary buffers
        string	head;
        string	tail;

		//find where the payload starts
		for(int i=start; i<length; i++)
		{
			//prevent the reader for thinking the 0 at the end of "rep / 726570" is part of the serialization
			if(	(buffer[i] == '0' && buffer[i+1] == '0' && buffer[i+2] == '0' && buffer[i+3] == '0') &&
				(buffer[i-1] != '7' && buffer[i-2] != '5' && buffer[i-3] != '6' && buffer[i-4] != '2'))
			{
				end = i-1;
				break;
			}
		}

		//finds command
		for(int i=start; i <= end; i++)
		{
			head += buffer[i];
		}

        //figures out how many characters to read next
		int 	search;
		string 	searchString;
		if(iterator == 3 && updatePacket["Contact-Id"] != "")
		{
			search = updatePacket["Contact-Id"].size() * 2;
		}
		else
		{
			for(int i=end+1; i<end+7; i++)
			{
				searchString += buffer[i];
			}
			search = engine.hex2Int(searchString)*2;
		}
		//+1 to the iterator
        iterator++;

		//gathers the value for the header
		for(int i=end+7; i< (end+7) + search; i++)
		{
			tail += buffer[i];
		}

        //Set new start character
		if(cipher.hexDec(head) == "contacts")
		{
			int newStart = end + 12 + (updatePacket["Contact-Id"].size() * 2);
			start = newStart+1;
		}
		else
		{
			start = end+7+search;
		}

		//save buffers to vectors
		if(cipher.hexDec(head) != "contacts")
        {
            updatePacket[cipher.hexDec(head)] = cipher.hexDec(tail);
        }

		//Are we done searching?
		if(start >= length-1 || end >= length-1)
		{
			running = false;
		}
    }

    if(engine.DEBUG)
    {
        if(updatePacket["Type"] == "1")
        {
            cout << "------------------------------------" << endl;
            cout << "Leave Packet" << endl;
            cout << "------------------------------------" << endl;

            cout << "Group-Id\t" << updatePacket["Group-Id"] << endl;
            cout << "Contact-Id\t" << updatePacket["Contact-Id"] << endl;
            cout << "------------------------------------" << endl;
        }
        else
        {
            cout << "------------------------------------" << endl;
            cout << "Join Packet" << endl;
            cout << "------------------------------------" << endl;

            cout << "Group-Id\t" << updatePacket["Group-Id"] << endl;
            cout << "Contact-Id\t" << updatePacket["Contact-Id"] << endl;

            cout << "------------------------------------" << endl;
            cout << "Contact Information" << endl;
            cout << "------------------------------------" << endl;
            cout << "Nickname\t" << updatePacket["Nickname"] << endl;
            cout << "Status\t\t" << updatePacket["Status"] << endl;
            cout << "Privileges\t" << updatePacket["Privileges"] << endl;
            cout << "rep\t\t" << updatePacket["rep"] << endl;
            cout << "Icon-Id\t\t" << updatePacket["Icon-Id"] << endl;
            cout << "Online-Status\t" << updatePacket["Online-Status"] << endl;
            cout << "Device-Type\t" << updatePacket["Device-Type"] << endl;
            cout << "------------------------------------" << endl;
        }
    }


    return updatePacket;
}
