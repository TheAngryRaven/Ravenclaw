#ifndef PALRINGODATABASE_H
#define PALRINGODATABASE_H

//project includes
#include "misc.h"
#include "crypt.h"
#include "curl.h"


class misc;
class crypt;
class curl;
class palringoDatabase
{
    public:
        palringoDatabase();

        map<string, string> logAdminAction(string group, string admin, string user, string action);

        map<string, string> userRegister(string user, string name);
        map<string, string> userLookUp(string user);

        map<string, string> groupRegister(string group, string name);
        map<string, string> groupLookUp(string group);

        map<string, string> logChat(string group, string user, string message);
    protected:
    private:
        misc	engine;
		crypt	cipher;
		curl    Curl;
};

#endif // PALRINGODATABASE_H
