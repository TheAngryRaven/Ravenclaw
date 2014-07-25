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

    output.addCommand("LOGON");
    output.addHeader("Client-ID", cipher.hexEnc(cipher.md5(email)));
    output.addHeader("Operator","PC_CLIENT");
    output.addHeader("affiliate-id","winpc");
    output.addHeader("app-type","Win/P7");
    output.addHeader("capabilities","4");
    output.addHeader("client-version: ","2.8.1, 60842");
    output.addHeader("fw","Win 6.2");
    output.addHeader("last","1");
    output.addHeader("name",email);
    output.addHeader("protocol-version","2.2");

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
	//output.addHeader("mesg-id",engine.i2s(id));
	output.addHeader("mesg-target",target);
	output.addHeader("target-id",to);
	output.addPayload(payload);
	return output;
}

packet palringoPacket::imageHeader(string target, string to, string mesgId, string length, string payload)
{
	engine.pl("palPacket-> creating IMAGE Header packet", 1);

	packet output;
	output.addCommand("MESG");
	output.addHeader("content-type","image/jpeg");
	output.addHeader("mesg-id",mesgId);
	output.addHeader("mesg-target",target);
	output.addHeader("target-id",to);
	output.addHeader("total-length",length);

    output.addPayload(payload);
	return output;
}

packet palringoPacket::image(string target, string to, string correlation, string mesgId, string payload)
{
	engine.pl("palPacket-> creating IMAGE packet", 1);

	packet output;
	output.addCommand("MESG");
	output.addHeader("content-type","image/jpeg");
	output.addHeader("correlation-id",correlation);
	output.addHeader("mesg-id",mesgId);
	output.addHeader("mesg-target",target);
	output.addHeader("target-id",to);

	output.addPayload(payload);
	return output;
}

packet palringoPacket::imageFinal(string target, string to, string correlation, string mesgId, string payload)
{
	engine.pl("palPacket-> creating IMAGE final packet", 1);

	packet output;
	output.addCommand("MESG");
	output.addHeader("content-type","image/jpeg");
	output.addHeader("correlation-id",correlation);
	output.addHeader("last","1");
	output.addHeader("mesg-id",mesgId);
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
	//output.addHeader("mesg-id","1");
	output.addHeader("target-id",target);
	return output;
}

packet palringoPacket::group_join(string group, string password)
{
	engine.pl("palPacket-> creating GROUP JOIN packet", 1);

	packet output;
	output.addCommand("GROUP SUBSCRIBE");
	//output.addHeader("mesg-id","1");
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
	//output.addHeader("mesg-id","1");
	output.addHeader("group-id",group);
	return output;
}

packet palringoPacket::ping(int number)
{
	engine.pl("palPacket-> creating PING packet", 1);
	packet output;

    //New? ping packet?
    output.addCommand("P");
    output.addHeader("last","1");
    output.addHeader("ps", engine.i2s(number) );

	return output;
}

packet palringoPacket::debug(string target, string to)
{
	engine.pl("palPacket-> creating debug packet", 1);

	packet output;
	output.addCommand("MESG");
	output.addHeader("content-type","text/plain");
	//output.addHeader("content-type","image/jpeghtml");
	output.addHeader("last","1");
	output.addHeader("mesg-id","1");
	output.addHeader("mesg-target",target);
	output.addHeader("target-id",to);
	//video packet
	//output.addPayload( cipher.hexDec("ffd9") + "<object type=\"application/x-shockwave-flash\" data=\"http://www.youtube.com/v/dw1HavgoK9E&fs=1\" width=\"480\" height=\"360\" id=\"object\"><param name=\"movie\" value=\"http://www.youtube.com/v/dw1HavgoK9E\" /><param name=\"quality\" value=\"high\" /><param name=\"allowFullScreen\" value=\"true\" /></object><script type=\"text/javascript\">if(window.navigator.userAgent.indexOf('iPhone')!=-1){var obj = document.getElementById('object'); obj.setAttribute('width',240); obj.setAttribute('height',180)}else if(window.navigator.userAgent.indexOf('iPad')==-1){var newspan = document.createElement('span');newspan.innerHTML='<a href=\"http://www.youtube.com/watch?v=dw1HavgoK9E\"><img src=\"http://img.youtube.com/vi/dw1HavgoK9E/0.jpg\" width=\"100%\" alt=\"[video]\" /></a>'; document.getElementById('object').appendChild(newspan)}</script>");
	output.addPayload( cipher.md5("dummy data") );
	return output;
}

string palringoPacket::generate_payload(string password, packet data)
{
	engine.pl("palPacket-> Generating AUTH", 1);

	//use this instead of the RNG
	string payload		= cipher.hexDec(data.getPayload());
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
	//return cipher.salsa20(IV, authKey, dte);
	string result = cipher.salsa20(IV, authKey, dte);

    if(engine.DEBUG)
    {
        cout << "------------------------------------" << endl;
        cout << "IV" << endl << cipher.hexEnc(IV) << endl << endl;
        cout << "AK" << endl << cipher.hexEnc(authKey) << endl << endl;
        cout << "DE" << endl << cipher.hexEnc(dte) << endl << endl;
        cout << "Auth inPacket" << endl << data.getPayload() << endl << endl;
        cout << "Auth outPacket" << endl << cipher.hexEnc(result) << endl;
        cout << "------------------------------------" << endl;
    }

	return result;
}
