/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

/*	Class: misc
 *	Contains simplistic functions such as pause, to easily use anywhere
 */

#ifndef MISC_H
#define MISC_H

//library includes
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string.h>
#include <conio.h>
#include <sstream>

using namespace std;

//project includes

struct fileDataStruct{
    vector< char > stream;
    long size;
    bool error;
    string status;
};

class misc
{
	public:
		misc();
		void  pause(void);			//pauses application/thread

        string i2s(int number);     //VERY basic int2string
		void  pf(string data,		//prints line to end of file
				 string file = "log.txt");

		void  pl(string data,		//prints line to console
				 int type = 0);

		void 			pLog(string data, int type);//prints packet data to log
		char*			str2ch(string data);	//input string receive char*
		vector<string>	splitStr(string data, char token);
		fileDataStruct  readFile(string fileLocation);

	protected:
	private:
};

#endif // MISC_H
