/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

#include "palringoContact.h"

palringoContact::palringoContact(palringoConnection *client, baseClient *base)
{
    palConn		= client;
	botClient	= base;
	botClient->set_palContact(this);
}

void palringoContact::client_add(map<string, string> data)
{
    contact newContact;

    newContact.nickname     = data["Nickname"];
    newContact.userId       = data["Contact-Id"];
    newContact.status       = data["Status"];
    newContact.privileges   = data["Privileges"];
    newContact.rep          = data["rep"];
    newContact.icon         = data["Icon-Id"];
    newContact.onlineStatus = data["Online-Status"];
    newContact.device       = data["Device-Type"];

    contactList[data["Contact-Id"]] = newContact;
    cout << "Added user ["<<data["Contact-Id"]<<"] to contact list" << endl;
}

contact palringoContact::client_lookup(string id)
{
    map<string, contact>::iterator it = contactList.find(id);

    if(it != contactList.end())
    {
       return contactList[id];
    }
    else
    {
        contact buffer;
        buffer.userId = "000";
        return buffer;
    }
}


void palringoContact::parse_subprofile(packet profilePacket)
{
    cout << "palContact-> Parsing Subprofile" << endl;


    cout << profilePacket.getPayload() << endl<<endl << endl;
    cout << cipher.hexDec(profilePacket.getPayload()) << endl;
}

wchar_t *palringoContact::CodePageToUnicode(int codePage, const char *src)
    {
    if (!src) return 0;
    int srcLen = strlen(src);
    if (!srcLen)
    {
    wchar_t *w = new wchar_t[1];
    w[0] = 0;
    return w;
    }
    
    int requiredSize = MultiByteToWideChar(codePage,
        0,
        src,srcLen,0,0);
    
    if (!requiredSize)
        {
        return 0;
        }
    
    wchar_t *w = new wchar_t[requiredSize+1];
    w[requiredSize] = 0;
    
    int retval = MultiByteToWideChar(codePage,
        0,
        src,srcLen,w,requiredSize);
    if (!retval)
        {
        delete [] w;
        return 0;
        }
    
    return w;
    }
    
char *palringoContact::UnicodeToCodePage(int codePage, const wchar_t *src)
    {
    if (!src) return 0;
    int srcLen = wcslen(src);
    if (!srcLen)
    {
    char *x = new char[1];
    x[0] = '\0';
    return x;
    }
    
    int requiredSize = WideCharToMultiByte(codePage,
        0,
        src,srcLen,0,0,0,0);
    
    if (!requiredSize)
        {
        return 0;
        }
    
    char *x = new char[requiredSize+1];
    x[requiredSize] = 0;
    
    int retval = WideCharToMultiByte(codePage,
        0,
        src,srcLen,x,requiredSize,0,0);
    if (!retval)
        {
        delete [] x;
        return 0;
        }
    
    return x;
    }