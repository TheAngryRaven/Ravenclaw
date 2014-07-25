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

    newContact.userId       = data["Contact-Id"];
    newContact.privileges   = data["Privileges"];
    newContact.icon         = data["Icon-Id"];
    newContact.rep          = data["rep"];
    newContact.nickname     = data["Nickname"];
    newContact.status       = data["Status"];
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


    //cout << profilePacket.getPayload() << endl<<endl << endl;
    //cout << cipher.hexDec(profilePacket.getPayload()) << endl;

    string testSubProfile = "333331373430333400006A50726976696C656765730000013149636F6E2D4964000001317265700000033333377265705F6C766C000008322E3735323730304E69636B6E616D6500001353697220526176656E636C6177205B626F745D53746174757300000F526176656E3A2032353930393736373131323500005C50726976696C65676573000008363836383538323549636F6E2D4964000002313272657000000531363431377265705F6C766C000008382E3335393030304E69636B6E616D6500000853616D616E746861636F6E7461637400000131323539303937363700006E50726976696C656765730000013149636F6E2D49640000023130726570000004323638327265705F6C766C000008352E3734353230304E69636B6E616D6500000652617665276E53746174757300001E4C65736269616E205472617070656420496E2061204D616E7320426F64793131333900006B50726976696C65676573000008363831363135333772657000000532343532367265705F6C766C000008382E3933383230304E69636B6E616D6500000B537570706F727420426F7453746174757300000F737570706F72742D6F2D6D61746963636F6E7461637400000131";

    cout << "testing subprofile decrypter" << endl;

    //cout << testSubProfile << endl;

    bool parsing = true;
    int packetLength = testSubProfile.length();
    int start = 0;
    int end = 0;

    while(parsing) {
        map<string, string> contact;

        std::size_t contactID = testSubProfile.find("0000");
        string tempSubProfile;

        if (contactID!=std::string::npos)
        {
            end = contactID;

            contact["Contact-Id"] = cipher.hexDec(testSubProfile.substr(start, end));

            string contactLength = testSubProfile.substr(end+1, 6);
            int cLength = engine.hex2Int(contactLength)*2;

            tempSubProfile = testSubProfile.substr(end+6,cLength);

            cout << "Contact-Id: " << contact["Contact-Id"] << endl;
        }

        //cout << "next: " << tempSubProfile << endl;


        std::size_t Privileges = tempSubProfile.find("50726976696C65676573");
        std::size_t Icon_Id = tempSubProfile.find("49636F6E2D4964");
        std::size_t rep = tempSubProfile.find("726570");
        std::size_t rep_lvl = tempSubProfile.find("7265705F6C766C");
        std::size_t Nickname = tempSubProfile.find("4E69636B6E616D65");
        std::size_t Status = tempSubProfile.find("537461747573");





        if (Privileges!=std::string::npos)
        {
            start = Privileges + 20;

            string pLength = tempSubProfile.substr(start, 6);

            string privilege = tempSubProfile.substr(start+6, engine.hex2Int(pLength)*2);

            contact["Privileges"] = cipher.hexDec(privilege);
            cout << "Privilege: " <<  contact["Privileges"] << endl;
        }

        if (Icon_Id!=std::string::npos)
        {
            start = Icon_Id + 14;

            string iLength = tempSubProfile.substr(start, 6);

            string icon = tempSubProfile.substr(start+6, engine.hex2Int(iLength)*2);

            contact["Icon-Id"] = cipher.hexDec(icon);
            cout << "Icon-Id: " <<  contact["Icon-Id"] << endl;
        }

        if (rep!=std::string::npos)
        {
            start = rep + 6;

            string rLength = tempSubProfile.substr(start, 6);

            string uRep = tempSubProfile.substr(start+6, engine.hex2Int(rLength)*2);

            contact["rep"] = cipher.hexDec(uRep);

            cout << "rep: " <<  contact["rep"] << endl;
        }

        if (rep_lvl!=std::string::npos)
        {
            start = rep_lvl + 14;

            string rlLength = tempSubProfile.substr(start, 6);

            string repLevel = tempSubProfile.substr(start+6, engine.hex2Int(rlLength)*2);

            contact["rep_lvl"] = cipher.hexDec(repLevel);

            cout << "rep_lvl: " <<  contact["rep_lvl"] << endl;
        }

        if (Nickname!=std::string::npos)
        {
            start = Nickname + 16;

            string nLength = tempSubProfile.substr(start, 6);

            string nick = tempSubProfile.substr(start+6, engine.hex2Int(nLength)*2);

            contact["Nickname"] = cipher.hexDec(nick);

            cout << "Nickname: " <<  contact["Nickname"] << endl;
        }

        if (Status!=std::string::npos)
        {
            start = Status + 12;

            string stLength = tempSubProfile.substr(start, 6);

            string uStatus = tempSubProfile.substr(start+6, engine.hex2Int(stLength)*2);

            contact["Status"] = cipher.hexDec(uStatus);

            cout << "Status: " <<  contact["Status"] << endl;
        }

        contact["Online-Status"] = "offline";
        contact["Device-Type"] = "offline";

        parsing = false;
        //this->client_add(contact);

        cout << "start: " << start << endl;
        cout << "length: " << packetLength << endl;
        if(start <= packetLength)
        {
            testSubProfile = testSubProfile.substr(start, packetLength - start);

            cout << testSubProfile << endl;
        }
        else
        {
            parsing = false;
        }
    }

}
