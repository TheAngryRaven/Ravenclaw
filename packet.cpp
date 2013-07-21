/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

#include "packet.h"

packet::packet()
{
	headCnt = 0;
	command = "";
	payload = "";
}

string packet::serialize()
{
	string output = "";
	string endLine = "\r\n"; //eaiser to create a uniform endline

	//add the command to the packet
	output.append(command);
	output.append(endLine);

	//discover the size of the payload
	int paySize = payload.size();
	if(paySize > 0)
	{
		stringstream oss;
		oss << paySize;

		output.append("content-length: ");
		output += oss.str();
		output.append(endLine);
	}

	//add headers
	for(int i=0; i < headCnt; i++)
	{
		if(headers[i][1].size() <= 0)
		{
			continue;
		}
		else
		{
			output.append(headers[i][0]);
			output.append(": ");
			output.append(headers[i][1]);
			output.append(endLine);
		}
	}

	//add payload
	output.append(endLine);
	output.append(payload);

	//and were done
	return output;
}

void packet::addHeader(string name, string data)
{
	headers[headCnt][0] = name;
	headers[headCnt][1] = data;

	headCnt ++;
}

string packet::search_headers(string query)
{
	for(int i=0; i<headCnt; i++)
	{
		if(headers[i][0] == query)
		{
			//string header = headers[i][1];
			//remove(header.begin(), header.end(),' ');
			return headers[i][1];
		}
	}

	return NULLHEADER;
}

void 			packet::addCommand(string data) { command  = data; }
void 			packet::addPayload(string data) { payload  = data; }

string			packet::getCommand()			{ return command; }
int				packet::getHeaders()			{ return headCnt; }
string  		packet::getPayload()			{ return payload; }
packetHeader	packet::getHeader(int id)
{
	packetHeader output;
	output.key   = headers[id][0];
	output.value = headers[id][1];
	return output;
}
