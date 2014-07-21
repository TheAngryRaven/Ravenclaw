/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

#include "palringoContact.h"

palringoContact::palringoContact(palringoConnection *client, baseClient *base)
{
    palConn		= client;
	botClient	= base;
	botClient->set_palContact(this);
}

void palringoContact::client_add(map<string, string> data)
{
    contact newContact;

    newContact.nickname     = data["Nickname"];
    newContact.userId       = data["Contact-Id"];
    newContact.status       = data["Status"];
    newContact.privileges   = data["Privileges"];
    newContact.rep          = data["rep"];
    newContact.icon         = data["Icon-Id"];
    newContact.onlineStatus = data["Online-Status"];
    newContact.device       = data["Device-Type"];

    contactList[data["Contact-Id"]] = newContact;
    cout << "Added user ["<<data["Contact-Id"]<<"] to contact list" << endl;
}

contact palringoContact::client_lookup(string id)
{
    map<string, contact>::iterator it = contactList.find(id);

    if(it != contactList.end())
    {
       return contactList[id];
    }
    else
    {
        contact buffer;
        buffer.userId = "000";
        return buffer;
    }
}


void palringoContact::parse_subprofile(packet profilePacket)
{
    cout << "palContact-> Parsing Subprofile" << endl;
}
