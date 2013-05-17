/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

#include "palringoPacket.h"

palringoPacket::palringoPacket(){}

packet palringoPacket::logon(string email)
{
	engine.pl("palPacket-> creating LOGON packet", 1);

	packet output;
/*
    Old Logon Info, still valid, but the other is the "standard"
	output.addCommand("LOGON");
	output.addHeader("protocol-version","2.0");
	output.addHeader("app-type","Windows x86");
	output.addHeader("Operator","PC_CLIENT");
	output.addHeader("name",email);
	output.addHeader("capabilities","4");
*/

    output.addCommand("LOGON");
    output.addHeader("Client-ID", "RavenClaw-bot");
    output.addHeader("Operator","winpc");
    output.addHeader("affiliate-id","winpc");
    output.addHeader("app-type","Windows x86");
    output.addHeader("capabilities","786437");
    output.addHeader("client-version: ","2.8.1, 60842");
    output.addHeader("fw","Win 6.2");
    output.addHeader("last","1");
    output.addHeader("name",email);
    output.addHeader("protocol-version","2.0");

	return output;
}

packet palringoPacket::auth(string password, packet data)
{
	engine.pl("palPacket-> creating AUTH packet", 1);

	packet output;

	output.addCommand("AUTH");
	output.addHeader("encryption-type","1");
	output.addHeader("online-status","1");

	string payloadBuffer = this->generate_payload(password, data);

	output.addPayload(payloadBuffer);

	//engine.pause();

	return output;
}

packet palringoPacket::message(string target, string to, string payload)
{
	engine.pl("palPacket-> creating MESG packet", 1);

	packet output;
	output.addCommand("MESG");
	output.addHeader("content-type","text/plain");
	output.addHeader("last","1");
	output.addHeader("mesg-id","1");
	output.addHeader("mesg-target",target);
	output.addHeader("target-id",to);
	output.addPayload(payload);
	return output;
}

packet palringoPacket::imageHeader(string target, string to, char* payload, int size)
{
	engine.pl("palPacket-> creating IMAGE packet", 1);

	stringstream imageSize;
	imageSize << size;

	packet output;
	output.addCommand("MESG");
	output.addHeader("content-type","image/jpeg");
	output.addHeader("last","1");
	output.addHeader("mesg-id","1");
	output.addHeader("mesg-target",target);
	output.addHeader("target-id",to);
	output.addHeader("total-length",imageSize.str());

    output.addPayload(payload);
	return output;
}

packet palringoPacket::image(string target, string to, char* payload)
{
	engine.pl("palPacket-> creating IMAGE packet", 1);

	packet output;
	output.addCommand("MESG");
	output.addHeader("content-type","image/jpeg");
	output.addHeader("last","1");
	output.addHeader("mesg-id","1");
	output.addHeader("mesg-target",target);
	output.addHeader("target-id",to);
	output.addPayload(payload);
	return output;
}

packet palringoPacket::admin(string action, string group, string target)
{
	engine.pl("palPacket-> creating ADMIN packet", 1);

	packet output;
	output.addCommand("GROUP ADMIN");
	output.addHeader("Action",action);
	output.addHeader("group-id",group);
	output.addHeader("last","1");
	output.addHeader("mesg-id","1");
	output.addHeader("target-id",target);
	return output;
}

packet palringoPacket::group_join(string group, string password)
{
	engine.pl("palPacket-> creating GROUP JOIN packet", 1);

	packet output;
	output.addCommand("GROUP SUBSCRIBE");
	output.addHeader("mesg-id","1");
	output.addHeader("Name",group);
	if(password != "")
	{
		output.addPayload(password);
	}
	return output;
}

packet palringoPacket::group_part(string group)
{
	engine.pl("palPacket-> creating GROUP PART packet", 1);

	packet output;
	output.addCommand("GROUP UNSUB");
	output.addHeader("mesg-id","1");
	output.addHeader("group-id",group);
	return output;
}

packet palringoPacket::ping(int number)
{
	engine.pl("palPacket-> creating PING packet", 1);
	packet output;

	/*
	Old ping packet
	output.addCommand("P");
    */

    /*
    New? ping packet?
    output.addCommand("P");
    output.addHeader("last","1");
    output.addHeader("ps", engine.i2s(number) );
    */

    output.addCommand("P");

	return output;
}

string palringoPacket::generate_payload(string password, packet data)
{
	engine.pl("palPacket-> Generating AUTH", 1);

	//use this instead of the RNG
	string payload		= data.getPayload();
	//string rnd		= data.getHeader(1).value.erase(10,1);
	string rnd			= data.search_headers("TIMESTAMP").erase(10,1);

	//create the IV
	char* IV = new char[9];
	for(int i=16; i<24; i++)
	{
		IV[i-16] = payload.c_str()[i];
	}
	IV[8] = '\0';

	//create some final keys for the salsa
	string authKey		= cipher.dbMD5(password, IV);
	//string sessionKey	= cipher.dbMD5(password, rnd);

	//finally create our data block to be hashed by salsa
	//this information get's hashed
	char* dte = new char[33];
	for(int i=0; i<16; i++)
	{
		dte[i]	= payload.c_str()[i];
		dte[i+16] = rnd.c_str()[i];
	}
	dte[32] = '\0';
	//string dataToEncrypt = dte;

	//generate final output for the packet
	//string result = cipher.salsa20enc(IV, authKey, dte);
	return cipher.salsa20(IV, authKey, dte);;
}
