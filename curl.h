/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

/*	Class: curl
 *	Info: handles gathering data from web locations
 */

#ifndef CURL_H
#define CURL_H

//project includes
#include "misc.h"
#include "crypt.h"

//library includes
#include <curl/curl.h>

class misc;
class crypt;
class curl
{
	public:
		curl();
		void    getUrl(string url);
		string  postUrl(string url, string data);
		string  buildPostData(map<string, string>postData);
		map<string, string> parseCurlPacket(string data);

	protected:
        //int writer(char *data, size_t size, size_t nmemb, string *buffer);

	private:
		misc    engine;
		crypt	cipher;

		CURL *curlP;
        CURLcode res;
};

#endif // CRYPT_H
