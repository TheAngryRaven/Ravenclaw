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

void palringoMessage::send_image(string target, string to, string imgPath)
{
    engine.pl("Attempting to send image", 1);

    fileDataStruct fileInput = engine.readFile(imgPath);

    if(!fileInput.error)
    {
        srand (time(NULL));
        int imgChunks   = fileInput.size/512;
        int leftOver    = fileInput.size - (imgChunks * 512);
        int messageId   = rand() % 40000 + 30000;
        string buffer;

        for(int i = 0; i != imgChunks+1; ++i)
        {
            buffer = "";
            if(i == 0)
            {
                //first packet
                for( int b = 0; b < 512; ++b )
                {
                    buffer += fileInput.stream[b];
                }

                palConn->send_packet(palPack.imageHeader(target, to, engine.i2s(messageId), engine.i2s(fileInput.size), buffer) );
            }
            else
            {
                //succeding packets
                int from = (512*i);
                int stop;

                if(i != imgChunks)
                    stop = from + 512;
                else
                    stop = from + leftOver;

                for( int b = from; b < stop; ++b )
                {
                    buffer += fileInput.stream[b];
                }

                if(i != imgChunks)
                    palConn->send_packet(palPack.image(target, to, engine.i2s(messageId), engine.i2s(messageId+i), buffer));
                else
                    palConn->send_packet(palPack.imageFinal(target, to, engine.i2s(messageId), engine.i2s(messageId+i), buffer));
            }
        }
    }
    else
    {
        this->send_message(target, to, "Error sending image:\r\n"+fileInput.status);
    }

}

void palringoMessage::send_message(string target, string to, string message)
{
	palConn->send_packet(palPack.message(target, to, message));
}

void palringoMessage::send_debug(string target, string to)
{
    palConn->send_packet(palPack.debug(target, to));
}
