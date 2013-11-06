/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

#ifndef REGEX_H
#define REGEX_H

//project includes
#include "misc.h"

//library includes
#include <regex/regex.h>

struct regex_return
{
	bool	success 	= false;
	string 	original 	= "This variable has not been initialized";
	string 	result 		= "";

	string 	pattern 	= "";
	string 	lookAhead 	= "";
	string 	lookBehind	= "";
	string 	fullPattern	= "";

	int		length		= 0;
	int		start		= 0;
	int		end			= 0;
};

class misc;
class regex
{
	public:


		regex_return search(string input, string pattern, string lookBehind = "", string lookAhead = "");
		int match(const char *string, char *pattern);

	protected:

	private:
};

#endif // REGEX_H
