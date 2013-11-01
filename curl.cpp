/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

/*
#include "curl.h"

//how we write data from CURL to buffer
int writer(char *data, size_t size, size_t nmemb, string *buffer)
{
    //fprintf(stderr,"Hello I am a function pointer\n");
    int result = 0;

    if(buffer != NULL)
    {
        buffer -> append(data, size * nmemb);
        result = size * nmemb;
    }

    return result;
}

curl::curl()
{
    curlP = curl_easy_init();
}
*/
/*
void curl::getUrl(string url)
{
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();

    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        //example.com is redirected, so we tell libcurl to follow redirection
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        //Perform the request, res will get the return code
        res = curl_easy_perform(curl);

        //Check for errors
        if(res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
        }

        //always cleanup
        curl_easy_cleanup(curl);
    }
}

string curl::postUrl(string url, string data)
{
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();

	string buffer;

    if(curl)
    {
        //set curl options
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_HEADER, 0);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }

    return buffer;
}

string curl::buildPostData(map<string, string>postData)
{
    string buffer;

    typedef map<string, string>::iterator it_type;

    for(it_type iterator = postData.begin(); iterator != postData.end(); iterator++)
    {
        if(iterator == postData.begin())
        {
            buffer += iterator->first + "=" +iterator->second;
        }
        else
        {
            buffer += "&"+iterator->first + "=" +iterator->second;
        }
    }

    return buffer;
}

//packet decrypt
map<string, string> curl::parseCurlPacket(string data)
{
   map<string, string> output;

    int packetLength    = data.length();
    const char* buffer  = data.c_str();

    string payload;
    int payloadStart = 0;

    //find where the payload starts
    for(int i=0; i<packetLength; i++)
    {
        if( buffer[i] == '0' &&
            buffer[i+1] == 'D' &&
            buffer[i+2] == '0' &&
            buffer[i+3] == 'A' &&
            buffer[i+4] == '0' &&
            buffer[i+5] == 'D' &&
            buffer[i+6] == '0' &&
            buffer[i+7] == 'A')
       {
           payloadStart = i+7;
           //cout << "Found payload, starting at " << payloadStart << endl;
           break;
       }
    }

    //If we have a valid payload
    if(payloadStart !=0)
    {

        //seperate the body
        string buff_head;
        string buff_value = "null";
        vector<string> headers;
        vector<string> values;
        bool header = true;


        //find headers and header values
        for(int i=0; i<=payloadStart-4; i++)
        {
            if(buffer[i] == '0' && buffer[i+1] == 'D' && buffer[i+2] == '0' && buffer[i+3] == 'A')
            {
                //if found \r\n stop operation and save to vector
                i += 3;
                headers.push_back(buff_head);
                values.push_back(buff_value);

                buff_head   = "";
                buff_value  = "";
                header      = true;
            }
            else if(buffer[i] == '3' && buffer[i+1] == 'A' && buffer[i+2] == '2' && buffer[i+3] == '0')
            {
                //if found : change writting to values instead of headers
                i +=3;
                header = false;
            }
            else
            {
                //write to the header or value buffer
                if(header)
                    buff_head += buffer[i];
                else
                    buff_value += buffer[i];
            }
        }

			output["command"] = cipher.hexDec(headers[0]);

            int headerCount = headers.size();

            for(int i=1; i<headerCount; i++)
            {
				output[cipher.hexDec(headers[i])] = cipher.hexDec(values[i]);
            }
    }
    else
    {
        output["command"] = "invalid";
    }

    return output;
}
*/

