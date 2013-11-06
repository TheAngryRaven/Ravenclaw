/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

#include "regex.h"

regex_return regex::search(string input, string pattern, string lookBehind, string lookAhead)
{
	//function vars
	regex_return 		buffer;
	int lookA 			= lookAhead.length();
	int lookB 			= lookBehind.length();

	int	start, end, length;
	string resultBuffer;

	//go ahead and set pattern data to output
	//this data wont change in the rest of the function anyway
	buffer.original		= input;
	buffer.pattern		= pattern;
	buffer.lookAhead	= lookAhead;
	buffer.lookBehind	= lookBehind;
	buffer.fullPattern	= lookBehind + pattern + lookAhead;

	//main regex vars
	int   		status;
	regex_t    	re;
	regmatch_t	matches;

	//merge/set inputs
	const char *re_input = input.c_str();

	//Generate full pattern with lookArounds
	char *re_pattern	= (char*)("("+lookBehind +")("+ pattern +")("+ lookAhead +")").c_str();

	//compile main pattern
	if (regcomp(&re, re_pattern, REG_EXTENDED) != 0)
	{
		//error compiling pattern
		buffer.original = "Error compiling pattern";
		return buffer;
	}

	//run the regular expression for the main pattern
	status = regexec(&re, re_input, (size_t)1, &matches, 0);

	//return results
	if (status != 0)
	{
		//match not found
		buffer.original = "Match not found";
		return buffer;
	}

	//set values after finding matches
	start			= matches.rm_so;
	end				= matches.rm_eo;
	length			= matches.rm_eo - matches.rm_so;

	resultBuffer	= input.substr(start, length);

	//We run lookArounds AFTER we search for the full expression, this way we can remove it from the result
	//lookAhead
	if(lookA <= 0)
	{
		lookA = 0;
	}
	else
	{
		int			lStatus;
		regex_t		lRe;
		regmatch_t	lMatch;
		char 		*lInput = (char*)lookAhead.c_str();

		//compile look ahead pattern
		if (regcomp(&lRe, lInput, REG_EXTENDED) != 0)
		{
			//error compiling pattern
			buffer.original = "Error compiling lookAhead";
			return buffer;
		}

		//run look ahead
		lStatus = regexec(&lRe, (char*)resultBuffer.c_str(), (size_t)1, &lMatch, 0);

		//return on no match
		if (lStatus != 0)
		{
			//match not found
			buffer.original = "LookAhead match not found";
			return buffer;
		}

		//found look ahead length
		lookA = lMatch.rm_eo - lMatch.rm_so;

		regfree(&lRe);
	}

	//lookBehind
	if(lookB <= 0)
	{
		lookB = 0;
	}
	else
	{
		int			lStatus;
		regex_t		lRe;
		regmatch_t	lMatch;
		char 		*lInput = (char*)lookBehind.c_str();

		//compile look ahead pattern
		if (regcomp(&lRe, lInput, REG_EXTENDED) != 0)
		{
			//error compiling pattern
			buffer.original = "Error compiling lookBehind";
			return buffer;
		}

		//run look ahead
		lStatus = regexec(&lRe, (char*)resultBuffer.c_str(), (size_t)1, &lMatch, 0);

		//return on no match
		if (lStatus != 0)
		{
			//match not found
			buffer.original = "LookBehind match not found";
			return buffer;
		}

		//found look ahead length
		lookB = lMatch.rm_eo - lMatch.rm_so;

		regfree(&lRe);
	}

	//free the pointer
	regfree(&re);

	//return data
	buffer.result	= resultBuffer.substr(lookB, resultBuffer.length() - (lookA + lookB));

	buffer.start	= (start + lookB);
	buffer.end		= (end - lookA);
	buffer.length	= (end - lookA) - (start + lookB);

	buffer.success	= true;

	return buffer;
}

int regex::match(const char *string, char *pattern)
{
	regex_t    		re;
	re_registers	reg;

	if (regcomp(&re, pattern, REG_EXTENDED) != 0)
	{
		return(0);      /* Report error. */
	}

	int derp = re_search(&re, string, sizeof(string), 0, 15, &reg);

	cout << "Result\t" << derp << endl;
	cout << "Found\t" << reg.num_regs << endl;
	cout << "Start\t" << reg.start << endl;
	cout << "End\t" << reg.end << endl;

	regfree(&re);

	return(1);
}
