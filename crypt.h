/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

/*	Class: crypt
 *	Info: contains various data ciphers and hashers
 *  all methods have been tested agaisnt known test data inputs and function correctly
 */

#ifndef CRYPT_H
#define CRYPT_H

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

//library includes
#include "cryptopp/cryptlib.h"
#include "cryptopp/salsa.h"
#include "cryptopp/md5.h"
#include "cryptopp/hex.h"
#include "cryptopp/osrng.h"

using namespace CryptoPP;

//project includes
#include "misc.h"

class misc;
class crypt
{
	public:
		crypt();

		//MD5 hash method
		string md5(string data);

		//HEX encryption and decryption methods
		string hexEnc(char* data, int count);
		string hexEnc(string data);
		string hexDec(string data);

		//salsa20 use the same method to encrypt/decrypt
		string salsa20(string IV, string KEY, string DATA);

		string dbMD5(string one, string two);
		string generateRandomBytes(int count);

	protected:
	private:
		misc engine;
};

#endif // CRYPT_H
