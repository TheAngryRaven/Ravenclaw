/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

#include "connection.h"

connection::connection(palringoConnection *client)
{
	palConn     = client;
	connected   = false;
}

unsigned long WINAPI readStub(void *ptr)
{
    if (!ptr) return -1;
    return ((connection*)ptr)->read();
}

bool connection::connectToHost(string IP, int PORT)
{
	engine.pl("connection-> Attempting connection", 1);

	WSADATA 		wsadata;
    SOCKADDR_IN 	target;

	//start up and check winsock
	int error = WSAStartup(0x0202, &wsadata);

    if (error)
        return connected;

    if (wsadata.wVersion != 0x0202)
    {
        WSACleanup(); //Clean up Winsock
        engine.pl("connection-> wrong winsock version", 1);
        return connected;
    }

	//set socket information
    target.sin_family		= AF_INET;
    target.sin_port			= htons(PORT);
    target.sin_addr.s_addr	= inet_addr(engine.str2ch(IP));

    //create socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//Couldn't create the socketmessage1
    if (sock == INVALID_SOCKET)
    {
    	engine.pl("connection-> Connection: Bad Socket", 1);

        return connected;
    }

    //try connecting to the socket
   	if(connect(sock, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
    {
		engine.pl("connection-> Could not connect", 1);

        return connected;
    }
    else
    {
    	engine.pl("connection-> Connection Established", 1);
    	engine.pl("connection-> Starting endless thread", 1);

    	//thread loops entire time and scans for server input
    	CreateThread(NULL, 0, readStub, this, 0, NULL);

		connected = true;
    	return connected;
    }
}

void connection::disconnect()
{
	if(sock)
		closesocket(sock);

	WSACleanup();

	connected = false;

	engine.pl("connection-> Connection Closed", 1);
}

void connection::writeBytes(char* data)
{
	engine.pl("connection-> sending data", 1);
	send(sock, data, sizeof(data), 0);
	engine.pLog(data, 1);
}

void connection::write(string data)
{
	engine.pl("connection-> sending data", 1);
	send(sock, data.c_str(), data.length(), 0);
	engine.pLog(data, 1);
}

DWORD connection::read(void)
{
	engine.pl("\nconnection-> Thread Started", 1);

	//setup data values
	int bufferSize = 10240;
	int bytes_recv = 0;
	char data_recv[bufferSize];

	//endless loop scanning for packets
	while(connected)
	{
		bytes_recv = recvfrom(sock, data_recv, bufferSize, 0,(struct sockaddr *) 0, (int *) 0);
		//struct sockaddr raw;
		//bytes_recv = recvfrom(sock, data_recv, sizeof(data_recv), 0,&raw, (int *) 0);

		//if we received data
		if(bytes_recv != -1)
		{
			string temp_buff = cipher.hexEnc(data_recv, bytes_recv);
			string buffer;

			buffer.append(temp_buff);
			//buffer.append(temp_buff.substr(0, temp_buff.size()));

			//engine.pLog(buffer, 0);
			//palConn->recv_packet(buffer, data_recv);
			palConn->recv_packet(buffer, data_recv);


			//reset buffers
			buffer = "";
			temp_buff = "";
			for(int i=0;i<bufferSize;i++)
				data_recv[i] = '\0'; //probably a better way to do this...
		}
	}

	engine.pl("connection-> lost connection, thread stopped", 1);

	return 0;
}

