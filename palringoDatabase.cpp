#include "palringoDatabase.h"

palringoDatabase::palringoDatabase()
{
    //ctor
}

string baseUrl = "http://127.0.0.1/palringo/";
map<string, string> palringoDatabase::logAdminAction(string group, string admin, string user, string action)
{
    map<string, string> postData;
    postData["command"]      = "add";
    postData["group"]        = group;
    postData["user"]         = admin;
    postData["target"]       = user;
    postData["action"]       = action;

    string output = Curl.postUrl(baseUrl+"adminLogs.php", Curl.buildPostData(postData));
    return Curl.parseCurlPacket(output);
}

map<string, string> palringoDatabase::userRegister(string user, string name)
{
    map<string, string> postData;
    postData["command"]     = "register";
    postData["user-id"]     = user;
    postData["nickname"]    = name;

    string output = Curl.postUrl(baseUrl+"users.php", Curl.buildPostData(postData));
    return Curl.parseCurlPacket(output);
}

map<string, string> palringoDatabase::userLookUp(string user)
{
    map<string, string> postData;
    postData["command"]     = "lookup";
    postData["user-id"]     = user;

    string output = Curl.postUrl(baseUrl+"users.php", Curl.buildPostData(postData));
    return Curl.parseCurlPacket(output);
}
