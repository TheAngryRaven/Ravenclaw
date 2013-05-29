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

void palringoGroup::group_update()
{
    botClient->group_update();
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
