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
#include <sstream>

#ifdef __MINGW32__
    #include <conio.h>
#endif

using namespace std;

//project includes
#include "INIreader.h"

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
		void            pause(void);			//pauses application/thread

        string          i2s(int number);     //VERY basic int2string
        string          l2s(long number);     //VERY basic long2string

        //prints line to end of file
		void            pf(string data,	string file = "log.txt");
        //prints line to consolke
		void            pl(string data, int type = 0);

        //prints packet data to log
		void 			pLog(string data, int type);

		//input string receive char*
		char*			str2ch(string data);

		//Splits string based on token character
		vector<string>	splitStr(string data, char token);

		//reads file to dinary and returns a special struct
		fileDataStruct  readFile(string fileLocation);

		//Insert hex string reveice calculated value
		int             hex2Int(string data);
		int             hexPow(int input, int power);

		bool DEBUG;

	protected:
	private:
};

#endif // MISC_H
