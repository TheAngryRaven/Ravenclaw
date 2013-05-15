/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

#include "misc.h"

//setting a few values
const bool DEBUG = true;
ofstream fileStream;

struct imgDataStruct //structure that holds file size and data stream
{
	long size;
	char *stream;
};


misc::misc(){}

void misc::pause()
{
	pl("\nPress any key to continue...");
	_getch();
}

//types
//0 = not debug output ( defualt )
//1 = debug output
void misc::pl(string data, int type)
{
	if((type == 1 && DEBUG == true) || type == 0)
		cout << data << endl;

	//if in debug mode print everything to the log file
	//if(DEBUG == true)
		//pf(data, "log.txt");
}

void misc::pf(string data, string file)
{
	fileStream.open(str2ch(file), ios::out | ios::app | ios::binary);
	fileStream << data << "\n";
	fileStream.close();
}

//types
//0 = recv
//1 = send
void misc::pLog(string data, int type)
{
	/*
	if(data.size() > 0)
	{
		char rw = 'u';

		if(type == 0)
			rw = 'R';
		else
			rw = 'W';

		fileStream.open("packets.xml", ios::out | ios::app | ios::binary);

		fileStream << "<" << rw << ">\n";
			fileStream << "<data>\n";
				fileStream << data;
			fileStream << "</data>\n";
		fileStream << "</" << rw << ">\n\n";

		fileStream.close();
	}
	*/
}

char* misc::str2ch(string data)
{
	/*
	int   dataLen = data.size();
	char* output  = new char[dataLen+1];
	output[dataLen] = '\0';
	memcpy(output, data.c_str(),dataLen);
	*/

	char* output = new char[data.length()];
	int leng = data.length();
	for(int i=0; i<leng; i++)
	{
		output[i] = data.c_str()[i];
	}

	return output;
}

vector<string> misc::splitStr(string data, char token)
{
	istringstream iss(data);
	string 			block;
	vector<string>	blocks;

	while(getline(iss, block, token))
	{
		std::transform(block.begin(), block.end(), block.begin(), ::tolower);
		blocks.push_back(block);
	}

	return blocks;
}

/*
imgDataStruct misc::readBinary(string fileLocation)
{
	//ilu cityfox <3
	FILE *pFile;
	long lSize;
	char *buffer;
	size_t result=0;

	pFile = fopen (fileLocation.c_str(), "rb");
	if (pFile==NULL)
	{
	//some event handling if the file doesn't exist or is used
	}

	//get file size
	fseek(pFile, 0, SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);

	//allocate memory for the stream
	buffer = (char*) malloc (sizeof(char)*lSize);
	if (buffer == NULL)
	{
	//some event handling for memory allocation error
	}

	// copy the file into the buffer:
	while(!feof(pFile)){result += fread(buffer, 1, lSize, pFile);}
	if (result != lSize)
	{
	//some event handling for read error
	}

	//terminate!!!
	fclose (pFile);
	imgDataStruct new_struct; //initialize structure that holds file size and data stream
	new_struct.stream=buffer;
	new_struct.size=lSize;

	return new_struct;
	free (buffer);
}

*/































