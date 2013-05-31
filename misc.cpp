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

misc::misc(){}

void misc::pause()
{
	pl("\nPress any key to continue...");
	_getch();
}

string misc::i2s(int number)
{

    stringstream ss;//create a stringstream
    ss << number;//add number to the stream
    return ss.str();//return a string with the contents of the stream
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

fileDataStruct misc::readFile(string fileLocation)
{
	vector< char > fileContents;
	fileDataStruct output;
	output.error = false;

    ifstream file(fileLocation.c_str(), ios::in | ios::binary | ios::ate);
    if(!file.is_open())
    {
        output.error = true;
        output.status = "Couldn't open file, does it exist?";
    }

    if(!output.error)
    {
        fileContents.resize(file.tellg());

        file.seekg(0, ios::beg);
        if(!file.read(&fileContents[ 0 ], fileContents.size()))
        {
            cout << "failed to read from image" << endl;
            output.error = true;
            output.status = "Failed to read file!";
        }


        if(!output.error)
        {
            output.size = fileContents.size();
            output.stream = fileContents;
        }
    }
    file.close();

    return output;
}
