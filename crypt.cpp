/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

/*
 * Crypto Library used: cryptopp
 * http://www.cryptopp.com/cryptopp561.zip
 * The Crypto++ Library (as a compilation) is currently licensed under the Boost
Software License 1.0 (http://www.boost.org/users/license.html)
 */

#include "crypt.h"

crypt::crypt(){}

string crypt::md5(string data)
{
	//engine.pl("crypt-> generating md5",1);
	//create encryptor
	CryptoPP::Weak::MD5 hash;
	byte digest[ CryptoPP::Weak::MD5::DIGESTSIZE ];

	//create hash digest
	//hash.CalculateDigest( digest, (byte*)data.c_str(), data.size() );
	hash.CalculateDigest( digest, (byte*)engine.str2ch(data), data.size() );

	//convert to hex data (16 byte to 32byte)
	CryptoPP::HexEncoder encoder;
	std::string output;
	encoder.Attach( new CryptoPP::StringSink( output ) );
	encoder.Put( digest, sizeof(digest) );
	encoder.MessageEnd();

	//convert output to lowercase
	//std::transform(output.begin(), output.end(), output.begin(), ::tolower);

	//return HEX data
	//palringo is niggers, convert MD5 back toHex
	return this->hexDec(output);
}

string crypt::hexEnc(char* data, int count)
{
	//engine.pl("crypt-> generating hex from plaintext",1);

	//create cipher
	CryptoPP::HexEncoder encoder;

	//create and write data to buffer
	string out;
	encoder.Attach(new CryptoPP::StringSink(out));
	encoder.Put((byte*)data, count);
	encoder.MessageEnd();

	return out;
}

string crypt::hexEnc(string data)
{
	//engine.pl("crypt-> generating hex from plaintext",1);

	//create cipher
	CryptoPP::HexEncoder encoder;

	//create and write data to buffer
	string out;
	encoder.Attach(new CryptoPP::StringSink(out));
	encoder.Put((byte*)engine.str2ch(data), data.size());
	encoder.MessageEnd();

	return out;
}

string crypt::hexDec(string data)
{
	//engine.pl("crypt-> generating plaintext from hex",1);

	//create cipher
	CryptoPP::HexDecoder decoder;

	//create and write data to buffer
	string out;
	decoder.Attach(new CryptoPP::StringSink(out));
	decoder.Put((byte*)engine.str2ch(data), data.length());
	decoder.MessageEnd();

	//return buffer
	return out;
}

string crypt::dbMD5(string one, string two)
{
	//generates a md5 with a "salt"
	//basically: md5( md5(one) , two )
	//engine.pl("crypt-> generating dbl md5",1);

	string ONE = md5(one);
	string hashIn = "";
	hashIn.append(ONE);
	hashIn.append(two);

	string result = md5(hashIn);

	return result;
}

string crypt::salsa20(string IV, string KEY, string DATA)
{
	//engine.pl("crypt-> encrypting to salsa20",1);

	//set up encryption
	Salsa20::Encryption enc;
	enc.SetKeyWithIV((byte*)engine.str2ch(KEY), KEY.length(), (byte*)engine.str2ch(IV));

	//create and move data to buffer
	string ENC = "";
	StreamTransformationFilter stf( enc, new StringSink( ENC ) );
	stf.Put( (byte*)DATA.c_str(), DATA.length() );
	stf.MessageEnd();

	//return buffer
	return ENC;
}

string crypt::generateRandomBytes(int count)
{
	//engine.pl("crypt-> generating random bytes",1);

	//generate random byte string
	int size = count/2;
	AutoSeededRandomPool rng;
	byte *randomBytes = new byte[size];
	rng.GenerateBlock(randomBytes, size);

	randomBytes[size] = '\0';

	string output = hexEnc((char*)randomBytes);

	int fSize  = output.size();
	if( fSize != count)
		engine.pl("crypt-> error creating rnd");

	return output;
}
