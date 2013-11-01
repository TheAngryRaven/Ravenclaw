/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

/*
#include "palringoDatabase.h"

palringoDatabase::palringoDatabase()
{
    //ctor
}

string baseUrl = "http://127.0.0.1/palringo/restapi/post";
map<string, string> palringoDatabase::logAdminAction(string group, string admin, string user, string action)
{
    map<string, string> postData;
    postData["class"]		= "adminLogs";
    postData["function"]	= "add";
    postData["group"]		= group;
    postData["user"]		= admin;
    postData["target"]		= user;
    postData["action"]		= action;

    string output = Curl.postUrl(baseUrl, Curl.buildPostData(postData));
    return Curl.parseCurlPacket(output);
}

map<string, string> palringoDatabase::userRegister(string user, string name)
{
    map<string, string> postData;
    postData["class"]		= "users";
    postData["function"]	= "register";
    postData["userid"]		= user;
    postData["nickname"]    = name;

    string output = Curl.postUrl(baseUrl, Curl.buildPostData(postData));
    return Curl.parseCurlPacket(output);
}

map<string, string> palringoDatabase::userLookUp(string user)
{
    map<string, string> postData;
    postData["class"]		= "users";
    postData["function"]	= "lookup";
    postData["userid"]     = user;

    string output = Curl.postUrl(baseUrl, Curl.buildPostData(postData));
    return Curl.parseCurlPacket(output);
}

map<string, string> palringoDatabase::groupRegister(string group, string name)
{
    map<string, string> postData;
    postData["class"]		= "groups";
    postData["function"]	= "register";
    postData["group"]		= group;
    postData["name"]   		= name;

    string output = Curl.postUrl(baseUrl, Curl.buildPostData(postData));
    return Curl.parseCurlPacket(output);
}

map<string, string> palringoDatabase::groupLookUp(string group)
{
    map<string, string> postData;
    postData["class"]		= "groups";
    postData["function"]	= "lookup";
    postData["group"]     	= group;

    string output = Curl.postUrl(baseUrl, Curl.buildPostData(postData));
    return Curl.parseCurlPacket(output);
}

map<string, string> palringoDatabase::logChat(string group, string user, string message)
{
    map<string, string> postData;
    postData["class"]		= "chatLogs";
    postData["function"]	= "add";
    postData["group"]		= group;
    postData["user"]		= user;
    postData["message"]		= message;

    string output = Curl.postUrl(baseUrl, Curl.buildPostData(postData));
    return Curl.parseCurlPacket(output);
}
*/
