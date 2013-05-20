/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

#include "palringoConnection.h"

//private variables
const string	SERVERIP	= "80.69.129.75"; //palringo
const int		SERVERPORT	= 0x3039;

palringoConnection::palringoConnection(palringoClient *client)
{
	palClient	= client;
	clientUser	= palClient->get_Client();
	loggedIn	= false;
	pingsSent   = 0;

	conn		= new connection(this);
	palMsg		= new palringoMessage(this, clientUser);
	palGroup		= new palringoGroup(this, clientUser);
}

bool palringoConnection::connect()
{
	engine.pl("palConn-> Connecting to "+SERVERIP);

	bool connection = conn->connectToHost(SERVERIP, SERVERPORT);

	if (connection)
	{
		engine.pl("palConn-> connection made");
		this->send_logon();
	}

	return connection;
}

void palringoConnection::disconnect()
{
	engine.pl("palConn-> Disconnecting");
	conn->disconnect();
}

void palringoConnection::recv_packet(string data, char* raw)
{
	//engine.pl("palConn-> packet received", 1);
	//engine.pl(data, 1);

	try
	{
		this->parse_recv(data, raw);
	}
	catch(...)
	{
		//engine.pl("palConn-> Error Parsing Packet Data", 1);
	}
}

void palringoConnection::send_packet(packet data)
{
	engine.pl("palConn-> sending packet", 1);

	string	buffer	= data.serialize();

	conn->write(buffer);
}

baseClient* palringoConnection::get_Client() { return clientUser; }

//private functions
void palringoConnection::send_logon()
{
	engine.pl("palConn-> sending LOGON packet");
	this->send_packet(palPack.logon(clientUser->get_Username()));
}

void palringoConnection::send_auth(packet data)
{
	engine.pl("palConn-> sending AUTH packet");
	this->send_packet(palPack.auth(clientUser->get_Password(), data));
}

void palringoConnection::send_ping()
{
	engine.pl("palConn-> sending PING packet", 1);
	pingsSent += 2; //not sure why do, its just what wireshark has shown me
	this->send_packet(palPack.ping(pingsSent));

    //Feature is currently depreciated
    //tries to keep the bot in a specific group
	//this->send_packet(palPack.group_join("groupname"));
}

//this function takes the string we get from our connection class
//and creates a packet that follows the standards we have already set


void palringoConnection::parse_recv(string data, char* raw)
{
	//engine.pl("palConn-> parsing input", 1);

	packet output;				//this is our output packet data

	istringstream iss(data);	//turns out data string, into a string stream
	string token;				//bascially used as a buffer, data is sent to and from
	vector<string> headers; 	//put all headers here for later logic
	int line = 0; 				//used basically to see which line we are pasrsing
	int headerCnt = 0;			//how many headers we have, used later

	while(getline(iss, token, '\n'))
	{
		if(line == 0)
		{
			//first line is the packet command
			//contains the damn \n char at the end so we remove it
			string cmd = token.substr(0, token.size()-1);

			if(cmd != "LOGON FAILED")
            {
                output.addCommand(cmd);
            }
			else
			{
				engine.pl("palConn-> Auth Failed");
				output.addCommand("LOGON FAILED");
				this->disconnect();
				Sleep(1000);
				this->connect();
				break;
			}
		}
		else
		{
			//the rest of the function are for headers
			//parses the colons in this to create the two parts for the header
			istringstream iss(token);
			string hTok;

			while(getline(iss, hTok, ':'))
			{
				//prevents parsing errors if a user sends a messagestarting with :
				if(hTok != "")
				{
					headers.push_back(hTok);
					headerCnt++;
				}
			}
		}

		line++;	//add one to our parsed lines list
	}

	//used to tell a later function if we need to add a payload
	for(int i=0; i < headerCnt; i += 2)
	{
		//we need to make sure to skip the last 3 parts of the vector
		//designed to handle every other itteration
		if(i < headerCnt-2)
		{
			//check if this is the CONTENT-LENGTH header
			if(headers[i] == "image/jpeg" || headers[i] == "IMAGE/JPEG")
			{
				break;
			}
			else if(headers[i] == "CONTENT-LENGTH" || headers[i] == "Content-Length")
			{
			    //beasue this is a COMPLETELY different packet type....
			    if(output.getCommand() == "GROUP UPDATE")
                {
                    /*
                    engine.pl("GU Content length found");
                    string headLen		= headers[i+1].substr(1, headers[i+1].size()-2);
                    int payloadLength	= atoi(headLen.c_str());

                    cout << "\n\nPayload Length: " << payloadLength << "\n\n";
                    cout << "\n\nPayload: " << cipher.hexEnc(raw) << "\n\n";

                    //crashes the parser, this is a "serialized" payload
                    //string tData = data.substr(data.size()-payloadLength, payloadLength);

                    //output.addPayload(tData);
                    */
                }
                else
                {
                    //adds the payload based on the content-length
                    //more accurate way of adding the payload
                    string headLen		= headers[i+1].substr(1, headers[i+1].size()-2);
                    int payloadLength	= atoi(headLen.c_str());

                    string tData = data.substr(data.size()-payloadLength, payloadLength);
                    output.addPayload(tData);
                }
			}
			else
			{
				//removes the last character from the second data set
				//the end line char is still there from the earlier parsing
				output.addHeader(headers[i], headers[i+1].substr(1, headers[i+1].size()-2));
			}
		}
	}

	//send the created packet to the packet parser
	//engine.pl("palConn-> Sending to packet parser", 1);
	this->parse_packet(output);
}

void palringoConnection::parse_packet(packet data)
{
	engine.pl("palConn-> parsing packet",1);

	int		paySize = data.getPayload().size();
	string	packCmd = data.getCommand();

    engine.pl("\n=====Packet=====",1);
    engine.pl(data.serialize(),1);
    engine.pl("================\n",1);

	if(packCmd == "AUTH")
	{
		engine.pl("palConn-> AUTH received");

		if(paySize == 24)
		{
			engine.pl("palConn-> AUTH valid");
			this->send_auth(data);
		}
		else
		{
			engine.pl("palConn-> AUTH invalid");
			this->disconnect();
			Sleep(1000);
			this->connect();
		}
	}
	else if(packCmd == "MESG")
	{
		palMsg->recv_message(data);
	}
	else if(packCmd == "GHOSTED")
    {
        engine.pl("palConn-> We got ghosted, disconnecting");
        loggedIn = false;
        this->disconnect();
    }
    else if(packCmd == "SUB PROFILE")
    {
        engine.pl("palConn-> Subprofile");
    }
    else if(packCmd == "GROUP ADMIN")
    {
        palGroup->group_admin(data);
    }
    else if(packCmd == "GROUP UPDATE")
    {
        palGroup->group_update();
    }
    else if(packCmd == "P")
    {
        engine.pl("palConn-> Received Ping Request", 1);
        this->send_ping();
    }
}
