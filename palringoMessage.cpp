/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

#include "palringoMessage.h"

palringoMessage::palringoMessage(palringoConnection *client, baseClient *base)
{
	palConn		= client;
	botClient	= base;
	botClient->set_palMesg(this);
}

void palringoMessage::recv_message(packet data)
{
 	if(data.search_headers("Content-Type") == "text/plain")
	{
		botClient->recv_message(data.search_headers("Target-Id"),data.search_headers("Source-Id"), data.getPayload());
	}
	else if(data.search_headers("CONTENT-TYPE") == "text/plain")
	{
		botClient->recv_pm(data.search_headers("EMAIL"), data.search_headers("SOURCE-ID"), data.getPayload());
	}
}

void palringoMessage::send_message(string group, string message)
{
	palConn->send_packet(palPack.message("1", group, message));
}

void palringoMessage::send_pm(string to, string message)
{
	palConn->send_packet(palPack.message("0", to, message));
}

void palringoMessage::admin_admin(string groupID, string user)
{
	palConn->send_packet(palPack.admin("1", groupID, user));
}

void palringoMessage::admin_mod(string groupID, string user)
{
	palConn->send_packet(palPack.admin("2", groupID, user));
}

void palringoMessage::admin_silence(string groupID, string user)
{
	palConn->send_packet(palPack.admin("8", groupID, user));
}

void palringoMessage::admin_reset(string groupID, string user)
{
	palConn->send_packet(palPack.admin("0", groupID, user));
}

void palringoMessage::admin_kick(string groupID, string user)
{
	palConn->send_packet(palPack.admin("16", groupID, user));
}

void palringoMessage::admin_ban(string groupID, string user)
{
	palConn->send_packet(palPack.admin("4", groupID, user));
}
