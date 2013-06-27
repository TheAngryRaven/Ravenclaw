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
ofstream fileStream;

misc::misc()
{
    DEBUG = true;
}

void misc::pause()
{
	pl("\nPress any key to continue...");

	#ifdef __MINGW32__
        _getch();
    #endif
}

string misc::i2s(int number)
{

    stringstream ss;//create a stringstream
    ss << number;//add number to the stream
    return ss.str();//return a string with the contents of the stream
}

string misc::l2s(long number)
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

int misc::hex2Int(string data)
{
    const char* input = data.c_str();
    int length = data.size();
    int buffer = 0;

    int it = 0;
    for(int i = length-1; i>=0; i--)
    {
        switch(input[i])
        {
            case '1': buffer = buffer + this->hexPow( 1, it); break;
            case '2': buffer = buffer + this->hexPow( 2, it); break;
            case '3': buffer = buffer + this->hexPow( 3, it); break;
            case '4': buffer = buffer + this->hexPow( 4, it); break;
            case '5': buffer = buffer + this->hexPow( 5, it); break;
            case '6': buffer = buffer + this->hexPow( 6, it); break;
            case '7': buffer = buffer + this->hexPow( 7, it); break;
            case '8': buffer = buffer + this->hexPow( 8, it); break;
            case '9': buffer = buffer + this->hexPow( 9, it); break;

            case 'A': buffer = buffer + this->hexPow(10, it); break;
            case 'B': buffer = buffer + this->hexPow(11, it); break;
            case 'C': buffer = buffer + this->hexPow(12, it); break;
            case 'D': buffer = buffer + this->hexPow(13, it); break;
            case 'E': buffer = buffer + this->hexPow(14, it); break;
            case 'F': buffer = buffer + this->hexPow(15, it); break;

            case 'a': buffer = buffer + this->hexPow(10, it); break;
            case 'b': buffer = buffer + this->hexPow(11, it); break;
            case 'c': buffer = buffer + this->hexPow(12, it); break;
            case 'd': buffer = buffer + this->hexPow(13, it); break;
            case 'e': buffer = buffer + this->hexPow(14, it); break;
            case 'f': buffer = buffer + this->hexPow(15, it); break;
        }
        it++;
    }
    return buffer;
}

int misc::hexPow(int input, int power)
{
    int buffer = 16;

    if(power > 0)
    {
        for(int i=1; i<power; i++)
        {
            buffer *= 16;
        }

        buffer *= input;
    }
    else
    {
        buffer = input;
    }

    return buffer;
}
