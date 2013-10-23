/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

#include "palringoConnection.h"
palringoConnection::palringoConnection(palringoClient *client, bool SSL)
{
    if(SSL == true)
    {
        engine.pl("palConn-> logging in with SSL", 1);
        SERVERIP = "80.69.129.123";
        SERVERPORT = 0x1BB; //443
    }
    else
    {
        engine.pl("palConn-> logging in with plaintext", 1);
        SERVERIP = "80.69.129.75";
        //SERVERPORT = 0x3039; //12345
        SERVERPORT = 0x9687; //38535
    }

	palClient	= client;
	clientUser	= palClient->get_Client();
	loggedIn	= false;
	pingsSent   = 0;
	messageId   = 1;

	conn		= new connection(this);
	palMsg		= new palringoMessage(this, clientUser);
	palGroup	= new palringoGroup(this, clientUser);
	palContact	= new palringoContact(this, clientUser);
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

    //add dynamic messageID to packet so we can track responses
    if(data.getCommand() == "MESG" ||
       data.getCommand() == "GROUP ADMIN" ||
       data.getCommand() == "GROUP SUBSCRIBE" ||
       data.getCommand() == "GROUP UNSUB")
    {
        string mesgId = engine.i2s(messageId);

        data.addHeader("mesg-id",mesgId);

        sentPackets[mesgId] = data;

        messageId++;
    }

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
}

//this function takes the string we get from our connection class
//and creates a packet that follows the standards we have already set


void palringoConnection::parse_recv(string data, char* raw)
{
    packet output;				//this is our output packet data

    //data = cipher.hexEnc(data);
    //cout << data << endl;

    int packetLength    = data.length();
    const char* buffer  = data.c_str();

    string payload;
    int payloadStart = 0;

    //find where the payload starts
    for(int i=0; i<packetLength; i++)
    {
        if( buffer[i] == '0' &&
            buffer[i+1] == 'D' &&
            buffer[i+2] == '0' &&
            buffer[i+3] == 'A' &&
            buffer[i+4] == '0' &&
            buffer[i+5] == 'D' &&
            buffer[i+6] == '0' &&
            buffer[i+7] == 'A')
       {
           payloadStart = i+7;
           //cout << "Found payload, starting at " << payloadStart << endl;
           break;
       }
    }

    //If we have a valid payload
    if(payloadStart !=0)
    {

        //seperate the body
        string buff_head;
        string buff_value = "null";
        vector<string> headers;
        vector<string> values;
        bool header = true;


        //find headers and header values
        for(int i=0; i<=payloadStart-4; i++)
        {
            if(buffer[i] == '0' && buffer[i+1] == 'D' && buffer[i+2] == '0' && buffer[i+3] == 'A')
            {
                //if found \r\n stop operation and save to vector
                i += 3;
                headers.push_back(buff_head);
                values.push_back(buff_value);

                buff_head   = "";
                buff_value  = "";
                header      = true;
            }
            else if(buffer[i] == '3' && buffer[i+1] == 'A' && buffer[i+2] == '2' && buffer[i+3] == '0')
            {
                //if found : change writting to values instead of headers
                i +=3;
                header = false;
            }
            else
            {
                //write to the header or value buffer
                if(header)
                    buff_head += buffer[i];
                else
                    buff_value += buffer[i];
            }
        }

            output.addCommand(cipher.hexDec(headers[0]));
            output.addPayload(cipher.hexDec(payload));


            int headerCount = headers.size();

            for(int i=1; i<headerCount; i++)
            {
                output.addHeader(cipher.hexDec(headers[i]), cipher.hexDec(values[i]));
                //cout << headers[i] << endl;
            }

        //This kills the payload
        for(int i=payloadStart+1; i<=packetLength; i++)
        {
            payload += buffer[i];
        }

        output.addPayload(payload);
    }
    else
    {
        output.addCommand("MALFORMED PACKET");
        output.addPayload(data);
    }

	this->parse_packet(output);
}

void palringoConnection::parse_packet(packet data)
{
	engine.pl("palConn-> parsing packet",1);

	int		paySize = data.getPayload().size()/2;
	string	packCmd = data.getCommand();

    if(engine.DEBUG == true && (packCmd != "MALFORMED PACKET" && packCmd != "SUB PROFILE" && packCmd != "BALANCE QUERY RESULT"))
    {
        //cout << engine.DEBUG << endl;
        //debug information
        cout << "------------------------------------" << endl;
        cout << "Command:\t\t";
        cout << data.getCommand() << endl;

        cout << "------------------------------------" << endl;

        cout << "[Body]" << endl;
        for(int i=1; i<data.getHeaders(); i++)
        {
            packetHeader buffer = data.getHeader(i);
            string tabs;
            int bSize = buffer.key.size();

            if(bSize < 8)
                tabs = "\t\t";
            else if(bSize >= 8 && bSize <= 11)
                tabs = "\t\t";
            else
                tabs = "\t";


            cout << "[" << i << "] " << buffer.key << tabs << buffer.value << endl;
        }

        cout << "------------------------------------" << endl;
        cout << "[Payload]" << endl << cipher.hexDec(data.getPayload()) << endl;
        cout << "------------------------------------" << endl;
        cout << "[Payload Raw]" << endl << data.getPayload() << endl;
        cout << "------------------------------------" << endl;
    }

	if(packCmd == "LOGON FAILED")
    {
        engine.pl("palConn-> Auth Failed");
        this->disconnect();
        Sleep(1000);
        this->connect();
    }
	else if(packCmd == "AUTH")
	{
		engine.pl("palConn-> AUTH received");

		if(paySize == 24)
		{
			engine.pl("palConn-> AUTH valid");

			data.addPayload(data.getPayload());

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
        engine.pl("palConn-> Received message");
	    data.addPayload(cipher.hexDec(data.getPayload()));
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
        engine.pl("palConn-> Subprofile received");
    }
    else if(packCmd == "GROUP ADMIN")
    {
        engine.pl("palConn-> Received Group Admin", 1);
        palGroup->group_admin(data);
    }
    else if(packCmd == "GROUP UPDATE")
    {
        engine.pl("palConn-> Received Group Update");
        palGroup->group_update(data);
    }
    else if(packCmd == "P")
    {
        engine.pl("palConn-> Received Ping Request", 1);
        this->send_ping();
    }
    else if(packCmd == "RESPONSE")
    {
        engine.pl("Received response packet");

        //once we receive the header pass both packets to baseclient to parse
        string mesgId = data.search_headers("MESG-ID");

        map<string, packet>::iterator it = sentPackets.find(mesgId);

        if(it != sentPackets.end())
        {
            this->clientUser->parseResponse(data, sentPackets[mesgId]);

            sentPackets.erase(it);
        }
    }
    else if(packCmd == "THROTTLE")
    {
        engine.pl("Received throttle packet");
        //this->clientUser->parseResponse(data);
    }
}
