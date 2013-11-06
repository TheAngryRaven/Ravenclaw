/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

#include "baseClient.h"

baseClient::baseClient(map<string, string> botSettings)
{
    //Login Details | No more compile time username and password
	this->username 	= botSettings["username"];
	this->password 	= botSettings["password"];

	this->botAdmin 	= botSettings["botAdmin"];  //the ID of the user who owns the bot
	this->botName	= botSettings["botName"];   //Set the bots name to give it personality
	this->adminName	= botSettings["adminName"]; //used for printing the admins name in messages
	this->botId	    = botSettings["botId"];     //used for certain checks to make sure your not targeting the bot itself, ill eventually figure out how to avoid this

	//these set the way you call commands
	//ie #leave or /help || #rc leave or #rc help
	this->nameSpace = botSettings["nameSpace"];

    //design the code so that we can either have or not have a namespace
    //IE: !rc test compared to !test
	if(this->nameSpace != "UNKNOWN")
    {
        this->cmdAdmin 	    = botSettings["cmdAdmin"] + this->nameSpace + " ";
        this->cmdBase	    = botSettings["cmdUser"]  + this->nameSpace + " ";
        this->patchStart    = 3;
        // !rc away message
        //  1   2      3
    }
    else
    {
		this->cmdAdmin 	    = botSettings["cmdAdmin"];
        this->cmdBase	    = botSettings["cmdUser"];
        this->patchStart 	= 2;
        // !away message
        //   1     2
    }

    //Feature is currently depreciated
	//controlGroup      = "[Group ID]"; //palringo
	//controlGroupName  = "[Group Name]"; //palringo

	//various "system" variables
	//Changed out canTalk works, it is no longer a global var to mute ALL sends
	//When the bot is muted i still want people to be able to use commands and such
	canTalk     = true;         //if the bot is muted or not
	adminOnline = false;        //if admin is online or not
	startTime   = time(NULL);   //sets the time the bot initializes
	security    = true;         //activates things like the group parser

	this->iniFile = botSettings["iniFile"];
	this->reloadIni();

	//Beta Features
	messagesReceived    = 0;
	messagesSent        = 0;
}

//triggered when a message is received
void baseClient::recv_groupMessage(string group, string user, string message)
{
    if(security && shouldMute(user))
    {
        this->admin_silence(group, user);

        this->send_message(group, "User <"+user+"> is a known pest");
        this->send_pm(botAdmin, "Attempted to Mute user <"+user+"> in group <"+group+">");

        if(botMods.size() > 0)
        {
            for(std::map<string, botMod>::iterator i = botMods.begin(); i != botMods.end(); ++i)
            {
                this->send_pm(i->first, "Attempted to Mute user <"+user+"> in group <"+group+">");
            }
        }
    }
    else if(security && shouldBan(user))
    {
        this->admin_ban(group, user);

        this->send_message(group, "User <"+user+"> is a known pest");
        this->send_pm(botAdmin, "Attempted to Ban user <"+user+"> in group <"+group+">");

        if(botMods.size() > 0)
        {
            for(std::map<string, botMod>::iterator i = botMods.begin(); i != botMods.end(); ++i)
            {
                this->send_pm(i->first, "Attempted to Ban user <"+user+"> in group <"+group+">");
            }
        }
    }
    else if(!isPest(user))
    {
        messagesReceived++;
        this->parse_groupMessage(group, user, engine.splitStr(message, ' '));
    }
}

//triggered when a PM is received
void baseClient::recv_personalMessage(string name, string user, string message)
{
    if(!isPest(user) and !shouldMute(user) and !shouldBan(user))
    {
        this->parse_personalMessage(name, user, engine.splitStr(message,' '));

        //palDB.userRegister(user, name);
    }
}

//triggered when anyone joins or leaves a group
void baseClient::group_update(map<string, string> updatePacket)
{
    string group = updatePacket["Group-Id"];

    if(updatePacket["Contact-Id"] != this->botId)
    {
        if(updatePacket["Type"] == "0")
        {
            string nickname = updatePacket["Nickname"];
            string userid = updatePacket["Contact-Id"];
            if(security && shouldMute(userid))
            {
                this->admin_silence(group, userid);
                this->send_message(group, "User <"+nickname+"> is a known pest");
            }
            else if(security && shouldBan(userid))
            {
                this->admin_ban(group, userid);
                this->send_message(group, "User <"+nickname+"> is a known pest");
            }
            else
            {
                this->send_message(group, nickname + " joined the group");
            }

			//palDB.userRegister(userid, nickname);
        }
        else
        {
            contact buffer = palContact->client_lookup(updatePacket["Contact-Id"]);

            if(buffer.userId != "000")
            {
                this->send_message(group, buffer.nickname + " left the group");
            }
            else
            {
				/*
				map<string, string> dbLookup = palDB.userLookUp(updatePacket["Contact-Id"]);
				if(dbLookup["success"] == "true")
				{
					this->send_message(group, dbLookup["nickname"]+" has left the group");
				}
				else
				{
					this->send_message(group, "Someone left the group");
				}
				*/

				this->send_message(group, "Someone left the group");
            }

        }
    }
}

//triggered when anyone commits an admin action
void baseClient::group_admin(string group, string admin, string user, string action)
{
    if(user != botId)
    {
        if(action == ACTION_ADMIN)
            this->send_message(group, "Congrats!\r\nJust dont power abuse");
        else if(action == ACTION_MOD)
            this->send_message(group, "Aww thats cute\r\nYou think you have power");
        else if(action == ACTION_SILENCE)
            this->send_message(group, "I love duct tape!");
        else if(action == ACTION_RESET)
            this->send_message(group, "Yay they reset you :3");
        else if(action == ACTION_KICK)
            this->send_message(group, "Get told");
        else if(action == ACTION_BAN)
            this->send_message(group, "Here comes the buthurt");
    }
    else
    {
        if(action == ACTION_ADMIN)
            this->send_message(group, "Thank you very much :3");
        else if(action == ACTION_MOD)
            this->send_message(group, "Woo I'm a cool kid now");
    }

    //palDB.logAdminAction(group, admin, user, action);
}

//ive left in some of the basic bot features im writting for my personal bot to give you a feel of how i do things
void baseClient::parse_groupMessage(string group, string user, vector<string> data)
{
	int     blocks  = data.size();
	string  mesg    = this->messagePatcher(data, " ", 1);

	//palDB.logChat(group, user, mesg);

    //new method of creating a command allows us to create "namespace commands" that palringo requires
    //IE: `#rc help` rather than `#help`
    string cmd;
    if(blocks >= patchStart-1)
    {
        if(this->nameSpace != "UNKNOWN")
        {
            cmd = data[0] + " ";

            if(cmd == cmdAdmin || cmd == cmdBase)
            {
               cmd = data[0] + " "+ data[1];
            }
            else
            {
                cmd = data[0];
            }
        }
        else
        {
            cmd = data[0];
        }
    }
    else
    {
        cmd = data[0];
    }

    //users online and bot security
    {
        //signs admin back online
        if(user == botAdmin && adminOnline == false)
        {
            //sets the bot admins status to being back online
            adminOnline = true;
            adminMessage = "";
            this->send_message(group, adminName+" is back online!");
        }

        //signs mods back online
        if(isMod(user))
        {
            if(botMods[user].name != "" and botMods[user].online == false)
            {
                botMods[user].online = true;
                botMods[user].message = "";
                this->send_message(group, botMods[user].name+" is back online!");
            }
        }

        //little security features like muting people who post group links
        if(security == true && user != botAdmin && !isMod(user))
        {
            //checks if someone posted a group link
            if(mesg.find("[") != std::string::npos && mesg.find("]") != std::string::npos)
            {
                this->send_message(group, "Don't post group links!");

                //buffer to save string to
                char const* buff = mesg.c_str();
                int length = strlen(buff);

                string groupName;
                int sPos = 0;
                int ePos = 0;

                //loop through the string and find the location of brackets
                for(int i=0; i < length; i++)
                {
                    if(buff[i] == '[')
                        sPos = i+1;

                    if(buff[i] == ']')
                        ePos = i;

                    if(sPos != 0 and ePos != 0)
                        break;
                }

                //if both brackets exit do some things
                if(ePos != 0)
                {
                    groupName = mesg.substr(sPos, ePos-sPos);
                    this->admin_silence(group, user);
                    this->send_message(group, "Don't make me come into `"+groupName+"` and spam it!");
                }
            }
        }
    }

    //admin commands
    if(user == botAdmin)
    {
        if(cmd == cmdAdmin+"check")
        {
            if(canTalk == true)
                this->send_message(group, "I am online!");
            else
            {
                //if the bots been muted this will still run
                canTalk = true;
                this->send_message(group, "I've been muted but im here ;_;");
                canTalk = false;
            }
        }
        else if(cmd == cmdAdmin+"leave")
        {
            this->group_part(group);
        }
        else if(cmd == cmdAdmin+"join")
        {
            if(blocks >= patchStart)
            {
                if(mesg.find("[") != std::string::npos && mesg.find("]") != std::string::npos)
                {
                    //buffer to save string to
                    char const* buffer = mesg.c_str();
                    int length = strlen(buffer);

                    string groupName;
                    string password;
                    int sPos = 0;
                    int ePos = 0;

                    //loop through the string and find the location of brackets
                    for(int i=0; i < length; i++)
                    {
                        if(buffer[i] == '[')
                            sPos = i+1;

                        if(buffer[i] == ']')
                            ePos = i;

                        if(ePos != 0)
                            break;
                    }

                    //if both brackets exit do some things
                    if(ePos != 0)
                    {
                        //set group name
                        groupName = mesg.substr(sPos, ePos-sPos);

                        //find password
                        if((sPos != 0 and ePos != 0) and (buffer[ePos+1] == ' '))
                            password = mesg.substr(ePos+2, length);
                        else
                            password = "";

                        //join the group
                        this->send_message(group, "Joining: "+groupName);
                        this->group_join(groupName, password);

                    }
                }
                else
                {
                    string groupName = this->messagePatcher(data, " ", patchStart);

                    this->send_message(group, "Joining: "+groupName);
                    this->group_join(groupName);
                }
            }
            else
            {
                this->send_message(group, cmdAdmin+"join [<group name>] <password>");
            }
        }
        else if(cmd == cmdAdmin+"msg")
        {
            if(blocks > patchStart)
            {
                //generate message
                string buff = this->messagePatcher(data, " ", patchStart+1);
                string user = data[patchStart-1];

                //send the buffer
                this->send_pm(user, buff);
                this->send_message(group, "Message: "+buff+"\r\nUser: "+user);
            }
            else
            {
                this->send_message(group, cmdAdmin+"msg <user ID> <message>");
            }
        }
        else if(cmd == cmdAdmin+"away")
        {
            /* Allows the bot admin to go afk, and the bot will know your not online, and leaves a message showing your not online */
            if(blocks > patchStart-1)
            {
                adminMessage	= this->messagePatcher(data, " ", patchStart);
                adminOnline		= false;
                this->send_message(group, "Bye " + adminName + "!\r\nAnd don't worry I saved the message\r\n"+adminMessage);
            }
            else
            {
                adminOnline = false;
                this->send_message(group, "Bye "+adminName+"!");
            }

        }
        else if(cmd == cmdAdmin+"mute")
        {
            if(canTalk == true)
            {
                this->send_message(group, "Muted myself");
                canTalk = false;
            }
            else
            {
                canTalk = true;
                this->send_message(group, "I can talk again!");
            }
        }
        else if(cmd == cmdAdmin+"secure")
        {
            if(security == true)
            {
                this->send_message(group, "Turning off security systems!");
                security = false;
            }
            else
            {
                security = true;
                this->send_message(group, "Turning on security systems");
            }
        }
        else if(cmd == cmdAdmin+"test")
        {
            //this->send_message(group, "Sending developer packet");
            //this->send_debug(group);

            //this->send_message(group, "Sending Transparency Test Image");
            //this->send_image(group, "transparency.png");
            //engine.getUrl("http://google.com");
            //curl.getUrl("http://google.com");

            //string buffer = Curl.postUrl("http://127.0.0.1/palringo/postTest.php","d=testdata");
            //this->send_message(group, buffer);
        }
        else if(cmd == cmdAdmin+"reload")
        {
            this->reloadIni(group);
        }
        else if(cmd == cmdAdmin+"info")
        {
            string buffer;
            string secure = "False";
            if(security == true)
            {
                secure = "True";
            }

            string mute = "False";
            if(this->canTalk != true)
            {
                mute = "True";
            }

            double uptime = difftime(time(NULL), startTime);

            char uptimeBuffer[256];
            int days    = uptime / 86400;
            int hours   = (uptime - 86400 * days) / 3600;
            int minutes = ((uptime - 86400 * days) - (3600 * hours))/60;
            int seconds = ((uptime - 86400 * days) - (3600 * hours))- 60 * minutes;

            //double days = (double)uptime/86400;
            snprintf(uptimeBuffer, sizeof(buffer), "%id %ih %im %is", days, hours, minutes, seconds);

            buffer +=   "Bot Information\r\n\r\n"
                        "Bot Admin: "+adminName+
                        "\r\nBot Name: "+botName+
                        "\r\nMods: "+engine.i2s(botMods.size())+
                        "\r\nSecurity Enabled: "+secure+
                        "\r\nBot Muted: "+mute+

                        "\r\nPests: "+engine.i2s(botPests.size())+
                        "\r\nAuto Ban: "+engine.i2s(banList.size())+
                        "\r\nAuto Mute: "+engine.i2s(muteList.size())+

                        "\r\nUptime: "+uptimeBuffer+
                        "\r\nMessages Received: "+engine.l2s(messagesReceived)+
                        "\r\nMessages Sent: "+engine.l2s(messagesSent);

            this->send_message(group, buffer);

        }
		/*
        else if(cmd == cmdAdmin+"register")
		{
			if(blocks > patchStart-1)
            {
                string groupName = this->messagePatcher(data, " ", patchStart);
                map<string, string> dbLookup = palDB.groupRegister(group, groupName);

                if(dbLookup["success"] == "true")
                {
                    this->send_message(group, "You have registered this group with the name <"+groupName+">");
                }
                else
                {
                    this->send_message(group, "There was an error registering the group");
                }
            }
            else
            {
                this->send_message(group, "Sorry thats not how you use this command\r\n"+cmdAdmin+"register <groupname>");
            }
		}
        else if(cmd == cmdAdmin+"group")
		{
			map<string, string> dbLookup = palDB.groupLookUp(group);

            if(dbLookup["success"] == "true")
            {
                this->send_message(group, "The group is currently registered under <"+dbLookup["name"]+">");
            }
            else
            {
                this->send_message(group, "This group is not currently registered to the bot\r\nPlease run the command "+cmdAdmin+"register <groupname>");
            }
		}
		*/

        else if(cmd == cmdAdmin+"help")
        {
             this->send_message(group, 	"Admin Help\r\n"+
                                        cmdAdmin+"check\r\n"+
                                        cmdAdmin+"leave\r\n"+
                                        cmdAdmin+"join [<group name>] <password>\r\n"+
                                        cmdAdmin+"msg <user id> <message>\r\n"+
                                        cmdAdmin+"away <message>\r\n"+
                                        cmdAdmin+"secure (also unsecure)\r\n"+
                                        cmdAdmin+"mute (also unmute)"+
                                        cmdAdmin+"test\r\n"+
                                        cmdAdmin+"reload\r\n"+
                                        cmdAdmin+"info"
										/*
										"\r\n"+cmdAdmin+"register <groupname>\r\n"+
                                        cmdAdmin+"group"
										*/
                                        );
        }
    }

    //mod commands
    if(isMod(user))
    {
        if(cmd == cmdAdmin+"check")
        {
            if(canTalk == true)
                this->send_message(group, "I am online!");
            else
            {
                //if shes been muted this will run
                canTalk = true;
                this->send_message(group, "I've been muted but im here ;_;");
                canTalk = false;
            }
        }
        else if(cmd == cmdAdmin+"leave")
        {
            this->group_part(group);
        }
        else if(cmd == cmdAdmin+"join")
        {
            if(blocks >= patchStart)
            {
                if(mesg.find("[") != std::string::npos && mesg.find("]") != std::string::npos)
                {
                    //buffer to save string to
                    char const* buffer = mesg.c_str();
                    int length = strlen(buffer);

                    string groupName;
                    string password;
                    int sPos = 0;
                    int ePos = 0;

                    //loop through the string and find the location of brackets
                    for(int i=0; i < length; i++)
                    {
                        if(buffer[i] == '[')
                            sPos = i+1;

                        if(buffer[i] == ']')
                            ePos = i;

                        if(ePos != 0)
                            break;
                    }

                    //if both brackets exit do some things
                    if(ePos != 0)
                    {
                        //set group name
                        groupName = mesg.substr(sPos, ePos-sPos);

                        //find password
                        if((sPos != 0 and ePos != 0) and (buffer[ePos+1] == ' '))
                            password = mesg.substr(ePos+2, length);
                        else
                            password = "";

                        //join the group
                        this->send_message(group, "Joining: "+groupName);
                        this->group_join(groupName, password);

                    }
                }
                else
                {
                    string groupName = this->messagePatcher(data, " ", patchStart);

                    this->send_message(group, "Joining: "+groupName);
                    this->group_join(groupName);
                }
            }
            else
            {
                this->send_message(group, cmdAdmin+"join [<group name>] <password>");
            }
        }
        else if(cmd == cmdAdmin+"msg")
        {
            if(blocks > patchStart)
            {
                //generate message
                string buff = this->messagePatcher(data, " ", patchStart+1);
                string user = data[patchStart-1];

                //send the buffer
                this->send_pm(user, buff);
                this->send_message(group, "Message: "+buff+"\r\nUser: "+user);
            }
            else
            {
                this->send_message(group, cmdAdmin+"msg <user ID> <message>");
            }
        }
        else if(cmd == cmdAdmin+"away")
        {
            /* Allows the bot admin to go afk, and the bot will know your not online, and leaves a message showing your not online */
            if(blocks > patchStart-1)
            {
				botMods[user].message	= this->messagePatcher(data, " ", patchStart);
				botMods[user].online	= false;
				this->send_message(group, "Bye " + botMods[user].name + "!\r\nAnd don't worry I saved the message");
            }
            else
            {
				botMods[user].online = false;
				this->send_message(group, "Bye "+botMods[user].name+"!");
            }

        }
        else if(cmd == cmdAdmin+"mute")
        {
            if(canTalk == true)
            {
                this->send_message(group, "Muted myself");
                canTalk = false;
            }
            else
            {
                canTalk = true;
                this->send_message(group, "I can talk again!");
            }
        }
        else if(cmd == cmdAdmin+"info")
        {
            string buffer;
            string secure = "False";
            if(security == true)
            {
                secure = "True";
            }

            string mute = "False";
            if(this->canTalk == true)
            {
                mute = "True";
            }

            double uptime = difftime(time(NULL), startTime);

            char uptimeBuffer[256];
            int days    = uptime / 86400;
            int hours   = (uptime - 86400 * days) / 3600;
            int minutes = ((uptime - 86400 * days) - (3600 * hours))/60;
            int seconds = ((uptime - 86400 * days) - (3600 * hours))- 60 * minutes;

            //double days = (double)uptime/86400;
            snprintf(uptimeBuffer, sizeof(buffer), "Uptime: %id %ih %im %is", days, hours, minutes, seconds);

            buffer +=   "Bot Information\r\n\r\n"
                        "Bot Admin: "+adminName+
                        "\r\nBot Name: "+botName+
                        "\r\nMods: "+engine.i2s(botMods.size())+
                        "\r\nSecurity Enabled: "+secure+
                        "\r\nBot Muted: "+mute+

                        "\r\nPests: "+engine.i2s(botPests.size())+
                        "\r\nAuto Ban: "+engine.i2s(banList.size())+
                        "\r\nAuto Mute: "+engine.i2s(muteList.size())+

                        "\r\nUptime: "+uptimeBuffer+
                        "\r\nMessages Received: "+engine.l2s(messagesReceived)+
                        "\r\nMessages Sent: "+engine.l2s(messagesSent);

            this->send_message(group, buffer);

        }
        else if(cmd == cmdAdmin+"help")
        {
             this->send_message(group, 	"Mod Help\r\n"+
                                        cmdAdmin+"check\r\n"+
                                        cmdAdmin+"leave\r\n"+
                                        cmdAdmin+"join [<group name>] <password>\r\n"+
                                        cmdAdmin+"msg <user id> <message>\r\n"+
                                        cmdAdmin+"away <message>\r\n"+
                                        cmdAdmin+"mute (also unmute)"+
                                        cmdAdmin+"info"
                                        );
        }
    }

    //user commands
    {
        //TODO: encode various text data for URL queries
        if(cmd == cmdBase+"admin")
        {
            string output = "The bot owner is "+adminName+" Userid: "+botAdmin;

            if(adminOnline)
                output += "\r\nAnd they are online";
            else
            {
                 output += "\r\nAnd they are offline";

                 if(adminMessage != "")
                    output += "\r\nBut they left a message!\r\n"+adminMessage;
            }

            this->send_message(group, output);
        }
        else if(cmd == cmdBase+"help")
        {
            this->send_message(group, 	"User Help\r\n"
                                        "< and > signs are used to show you command arguments, do not type them out!\r\n"+
                                        cmdBase+"admin\r\n"+
                                        cmdBase+"mods\r\n"+
                                        cmdBase+"google <query>\r\n"+
                                        cmdBase+"youtube <query>\r\n"+
                                        cmdBase+"credits\r\n"+
                                        cmdBase+"uptime\r\n"+
                                        cmdBase+"dice <coin,6,8,10,12 or 20>\r\n"+
                                        cmdBase+"website"
										#ifdef RAVENCLAW_DEBUG
                                        "\r\n"+cmdBase+"nickname 'Shows your current registered nickname'\r\n"+
                                        cmdBase+"register <nickname>"
										#endif
                                        );
        }
        else if(cmd == cmdBase+"credits")
        {
            this->send_message(group, 	"Ravenclaw bot created by Raven [furry]\r\n\r\n"
                                        "I can thank so many people on pal for this\r\n"
                                        "Mainly ross, sniper, and nom\r\n"
                                        "And my friend city fox for a binary file reader\r\n"
                                        "the rest know who you are\r\n");
        }
        else if(cmd == cmdBase+"website")
        {
            this->send_message(group, "This bot is open source under GPLv3"
                                        "\r\nRepo is now permanently public\r\n"
                                        "\r\nRaven in [furry] would GLADLY teach you how to compile it\r\n"
                                        "https://bitbucket.org/BlackRaven/ravenclaw/");
        }
        else if(cmd == cmdBase+"google")
        {
            if(blocks > patchStart-1)
            {
                string search = "Here you go\r\nhttp://www.google.com/search?q=";
                search.append(this->messagePatcher(data, "+", patchStart));
                this->send_message(group, search);
            }
            else
            {
                this->send_message(group, "Sorry thats not how you use this command\r\n"+cmdBase+"google <search query>");
            }
        }
        else if(cmd == cmdBase+"youtube")
        {
            if(blocks > patchStart-1)
            {
                string search = "Videos are fun!\r\nhttp://www.youtube.com/results?search_query=";
                search.append(this->messagePatcher(data, "+", patchStart));
                this->send_message(group, search);
            }
            else
            {
                this->send_message(group, "Sorry hun thats not how you use this command\r\n"+cmdBase+"youtube <search query>");
            }
        }
        else if(cmd == cmdBase+"dice")
        {
            if(blocks == patchStart)
            {
                srand (time(NULL));
                int dieRoll = 0;
                string coin = "";
                string diceType = data[patchStart-1];


                if(diceType == "coin")
                {
                    dieRoll = rand() % 50 + 1;

                    if(dieRoll < 25)
                        coin = "heads";
                    else
                        coin = "tails";
                }
                else if(diceType == "6" || diceType == "d6")
                    dieRoll = rand() % 6 + 1;
                else if(diceType == "8" || diceType == "d8")
                    dieRoll = rand() % 8 + 1;
                else if(diceType == "10" || diceType == "d10")
                    dieRoll = rand() % 10 + 1;
                else if(diceType == "12" || diceType == "d12")
                    dieRoll = rand() % 12 + 1;
                else if(diceType == "20" || diceType == "d20")
                    dieRoll = rand() % 20 + 1;

                if(dieRoll != 0 && coin == "")
                {
                    this->send_message(group, "And the DM rolls a "+engine.i2s(dieRoll));
                }
                else if(coin != "")
                {
                    this->send_message(group, "And its "+coin);
                }
                else
                {
                    this->send_message(group, "Not a valid dice type");
                }
            }
            else
            {
                this->send_message(group, "Sorry thats not how you use this command\r\n"+cmdBase+"dice <coin,6,8,10,12 or 20>");
            }
        }
        else if(cmd == cmdBase+"mod" or cmd == cmdBase+"mods")
        {
            if(botMods.size() > 0)
            {
                string output = "The bot now has mods\r\n";

                for(std::map<string, botMod>::iterator i = botMods.begin(); i != botMods.end(); ++i)
                {
                    output.append("\r\n"+i->second.name);

                    if(i->second.online)
                    {
                        output.append(" - online\r\n");
                    }
                    else
                    {
                        output.append(" - offline\r\n");

                        if(i->second.message != "")
                            output.append("Message: "+i->second.message+"\r\n");
                    }
                }

                this->send_message(group, output);
            }
            else
                this->send_message(group, "The bot currently has no mods assigned");
        }
        else if(cmd == cmdBase+"uptime")
        {
            double uptime = difftime(time(NULL), startTime);

            char buffer[256];
            int days    = uptime / 86400;
            int hours   = (uptime - 86400 * days) / 3600;
            int minutes = ((uptime - 86400 * days) - (3600 * hours))/60;
            int seconds = ((uptime - 86400 * days) - (3600 * hours))- 60 * minutes;

            //double days = (double)uptime/86400;
            snprintf(buffer, sizeof(buffer), "Uptime: %id %ih %im %is", days, hours, minutes, seconds);

            this->send_message(group, buffer);
        }
        else if(cmd == cmdBase+"translate")
		{
			if(blocks > patchStart+1)
            {
				string URL = "http://translate.google.com/translate_a/t?client=s&sl="+data[patchStart-1]+"&tl="+data[patchStart]+"&q=";
                URL.append(this->messagePatcher(data, "+", patchStart+2));

				string request = Curl.getUrl(URL);

				string LB_Pattern = "trans\":\"";
				string Pattern = ".[A-Za-z ,!.]*.+";
				string LA_Pattern = "\",\"orig";

				string result = "";
				size_t start = request.find(LB_Pattern) + LB_Pattern.length();
				size_t stop = request.find(LA_Pattern);

				if(start != std::string::npos && stop != std::string::npos)
				{
					result = request.substr(start, stop-start);
				}
				else
				{
					result = "cannot find translation";
				}
				this->send_message(group, result);
            }
            else
            {
                this->send_message(group, "Sorry hun thats not how you use this command\r\n"+cmdBase+"translate <from> <to>\r\nExample "+cmdBase+"translate en fr hello my lady");
            }
		}
		/*
        else if(cmd == cmdBase+"nickname")
        {
            map<string, string> dbLookup = palDB.userLookUp(user);

            if(dbLookup["success"] == "true")
            {
                this->send_message(group, "You are currently registered under <"+dbLookup["nickname"]+">");
            }
            else
            {
                this->send_message(group, "You are not currently registered to the bot\r\nPlease run the command "+cmdBase+"register <nickname>");
            }
        }
        else if(cmd == cmdBase+"register")
        {
            if(blocks > patchStart-1)
            {
                string nickname = this->messagePatcher(data, " ", patchStart);
                map<string, string> dbLookup = palDB.userRegister(user, nickname);

                if(dbLookup["success"] == "true")
                {
                    this->send_message(group, "You have registered the nickname <"+nickname+">");
                }
                else
                {
                    this->send_message(group, "There was an error registering your nickname");
                }
            }
            else
            {
                this->send_message(group, "Sorry thats not how you use this command\r\n"+cmdBase+"register <nickname>");
            }
        }
		*/
    }

    //Bot AI
    if(cmd == botName && canTalk) /* "talk to the bot" */
	{
		if(blocks >= 2)
		{
			string aiCommand = this->messagePatcher(data, " ", 2);

			//command phrases
			//such basic AI haha
			if(aiCommand == "is funny")
			{
				if(user == botAdmin)
				{
					this->send_message(group, "Of course I am");
				}
				else
				{
					this->send_message(group, "im not just funny im amazing :3");
				}
			}
			else if(aiCommand == "how are you" || aiCommand == "how are you?")
            {
                if(user == botAdmin)
				{
					this->send_message(group, "You know im doing good, but thanks for asking");
				}
				else
				{
					this->send_message(group, "I'm good thanks for asking!");
				}
            }
			else if(aiCommand == "is cute")
			{
				if(user == botAdmin)
				{
					this->send_message(group, "Thank you "+botAdmin+" :3");
				}
				else
				{
					this->send_message(group, "i know im cute");
					this->send_message(group, "im flattered\r\nbut seriously\r\nim just some computer code");
					this->send_message(group, "#android-problems");
				}
			}
			else if(aiCommand == "is ugly")
			{
				if(user == botAdmin)
				{
					this->send_message(group, "Stop being a bitch "+botAdmin);
				}
				else
				{
					this->send_message(group, "/me cries");
					this->send_message(group, "You're a bitch >:C");
				}
			}
		}
		else
		{
			this->send_message(group, "Hello :3");
		}
	}

	//Word filters and other shit
	else if(canTalk)
	{
		if(blocks >= 1) /* phrase filters */
		{
			//generates our phrase with our magical messagePatcher <3
			string phrase = this->messagePatcher(data, " ", 1);

			if(phrase == "bye "+botName)
			{
				this->send_message(group, "see ya soon :D");
			}
			else if(phrase == "bye guys")
			{
				this->send_message(group, "/me waves goodbye");
			}
			else /* word filters, triggers if the word is anywhere in the users post */
			{
				//word trigger
				//remember to break at the end of each if
				//for the love of god dont forget
				for(int i=0; i<blocks; i++)
				{
					//Hash Tags, triggers if someone uses a hashtag
					if(i==(blocks-1) && engine.str2ch(data[i])[0] == '#')
					{
						string hash = data[i];
						if(hash == "#poniproblems")
						{
							this->send_message(group, "Fuck you, and fuck your ponies");
							this->admin_silence(group, user);
						}
						else if(hash == "#yolo")
                        {
                            this->send_message(group, "NIGGA\r\nYou only live fuckin once\r\nSo stop getting fucking shitfaced every damn day because YOU HAVE A LIFE TO LIVE NIGGA");
                        }
						else if(hash == "#swag")
						{
							this->send_message(group, "Man check you out, fuckin swaggin so hard, the term swag itself comes from how swaggin you are.");
						}

						break;
					}
					else if( searchMod(data[i]) != "" )
                    {
                        string output;

                        //save to a temp botMod instance so we dont have to keep searching
                        botMod temp = botMods.find(searchMod(data[i]))->second;

                        if(temp.online)
                        {
                            output += data[i]+" is online!";
                        }
                        else
                        {
                            output += data[i]+" is offline!";

                            if(temp.message != "")
                                output += "\r\nBut they left of a message!\r\n"+temp.message;
                        }

                        this->send_message(group, output);

                        break;
                    }
					else if(data[i] == "u" || data[i] == "c" || data[i] == "r" || data[i] == "y" || data[i] == "o")
					{
						this->send_message(group, "Use words not letters\r\nDon't be lazy!");
						//this->admin_silence(group, user);
						break;
					}
					/*else if(data[i] == "ass" || data[i] == "bitch" || data[i] == "fuck" || data[i] == "shit" || data[i] == "damn" || data[i] == "hell" || data[i] == "cunt")
					{
						this->send_message(group, "Potty Mouth!!!");
						//this->admin_silence(group, user);
						break;
					}*/
					else if(data[i] == "cod" || (i<blocks-2 && data[i] == "call" && data[i+1] == "of" && data[i+2] == "duty"))
					{
						/* can also detect phrases anywhere in the post */
						if(user != botAdmin)
						{
							this->send_message(group, 	"it would be awesome if you didnt talk about that shitty game\r\n"
														"It has the most inaccurate presntations of firearms second only to doom\r\n"
														"Brink has more accurate weapons than COD\r\n"
														"The ak74 isnt a fucking SMG");
							break;
						}
					}
				}
			}
		}
	}

}

//seperate command parser for pms
void baseClient::parse_personalMessage(string name, string user, vector<string> data)
{
	int		blocks		= data.size();
	string  mesg        = this->messagePatcher(data, " ", 1);

    string cmd;
    if(blocks >= patchStart-1)
    {
        if(this->nameSpace != "UNKNOWN")
        {
            cmd = data[0] + " ";

            if(cmd == cmdAdmin || cmd == cmdBase)
            {
               cmd = data[0] + " "+ data[1];
            }
            else
            {
                cmd = data[0];
            }
        }
        else
        {
            cmd = data[0];
        }
    }
    else
    {
        cmd = data[0];
    }

    if(user == botAdmin or isMod(user))
    {
        if(cmd == cmdAdmin+"msg")
        {
            if(blocks > patchStart)
            {
                //generate message
                string buff = this->messagePatcher(data, " ", patchStart+1);
                string user = data[patchStart-1];

                //send the buffer
                this->send_pm(user, buff);
            }
            else
            {
                this->send_pm(user, cmdAdmin+"msg <user ID> <message>");
            }
        }
        if(cmd == cmdAdmin+"uptime")
        {
            double uptime = difftime(time(NULL), startTime);

            char buffer[256];
            int days    = uptime / 86400;
            int hours   = (uptime - 86400 * days) / 3600;
            int minutes = ((uptime - 86400 * days) - (3600 * hours))/60;
            int seconds = ((uptime - 86400 * days) - (3600 * hours))- 60 * minutes;

            //double days = (double)uptime/86400;
            snprintf(buffer, sizeof(buffer), "Uptime: %id %ih %im %is", days, hours, minutes, seconds);

            this->send_pm(user, buffer);
        }
        if(cmd == cmdAdmin+"leave")
        {
            if(blocks == patchStart)
            {
                this->group_part(data[patchStart-1]);
            }
            else
            {
                this->send_pm(user, cmdAdmin+"leave <group id>");
            }
        }
        if(cmd == cmdAdmin+"join")
        {
            if(blocks >= patchStart)
            {
                if(mesg.find("[") != std::string::npos && mesg.find("]") != std::string::npos)
                {
                    //buffer to save string to
                    char const* buffer = mesg.c_str();
                    int length = strlen(buffer);

                    string groupName;
                    string password;
                    int sPos = 0;
                    int ePos = 0;

                    //loop through the string and find the location of brackets
                    for(int i=0; i < length; i++)
                    {
                        if(buffer[i] == '[')
                            sPos = i+1;

                        if(buffer[i] == ']')
                            ePos = i;

                        if(ePos != 0)
                            break;
                    }

                    //if both brackets exit do some things
                    if(ePos != 0)
                    {
                        //set group name
                        groupName = mesg.substr(sPos, ePos-sPos);

                        //find password
                        if((sPos != 0 and ePos != 0) and (buffer[ePos+1] == ' '))
                            password = mesg.substr(ePos+2, length);
                        else
                            password = "";

                        //join the group
                        this->send_pm(user, "Joining: "+groupName);
                        this->group_join(groupName, password);

                    }
                }
                else
                {
                    string groupName = this->messagePatcher(data, " ", patchStart);

                    this->send_pm(user, "Joining: "+groupName);
                    this->group_join(groupName);
                }
            }
            else
            {
                this->send_pm(user, cmdAdmin+"join [<group name>] <password>");
            }
        }
        if(cmd == cmdAdmin+"help")
        {
            this->send_pm(user, 	"Admin Help\r\n"+
                                    cmdAdmin+"msg <user id> <message>\r\n"+
                                    cmdAdmin+"join [<group name>] <password>\r\n"+
                                    cmdAdmin+"leave <group id>\r\n"+
                                    cmdAdmin+"uptime\r\n"
                                    );
        }
    }
    else
    {
        string buffer = "";
		buffer.append("====== PM Received ======\r\n");
		buffer.append("From: ");
		buffer.append(name);
		buffer.append("\r\n");
		buffer.append(mesg);
		buffer.append("\r\n=======================\r\n");
		this->send_pm(botAdmin, buffer); //sends the botadmin the pm it received

		buffer = "";
		buffer.append("#msg ");
		buffer.append(user);
		buffer.append(" ");
		this->send_pm(botAdmin, buffer); //sends a second message so the admin can copy/paste the command for eaiser replies
    }
}

//BETA
//handle response packets
void baseClient::parseResponse(packet response, packet sent)
{
    string resp_what = response.search_headers("WHAT");
    string resp_code = engine.i2s(engine.hex2Int(response.getPayload().substr (12,4)));

    //debug info
    if(engine.DEBUG)
    {
        string buffer = "What: "+resp_what+"\r\nCode: "+resp_code;

        cout << "------------------------------------" << endl;
        cout << buffer << endl;
        cout << "------------------------------------" << endl;
    }

    //messages
    if(resp_what == "11")
    {
        if(resp_code == "13")
        {
            this->send_pm(botAdmin, "Failed sending message\r\n\r\nType: "+sent.search_headers("mesg-target")+"\r\nTarget: "+sent.search_headers("target-id")+"\r\nMessage: "+sent.getPayload());
        }
    }

    //Joining Group
    if(resp_what == "1")
    {
        if(resp_code == "2")
        {
            this->send_pm(botAdmin, "Failed joining ["+ sent.search_headers("Name") +"]\r\n\r\ndoes not exist");
        }
        else if(resp_code == "13")
        {
            this->send_pm(botAdmin, "Failed joining ["+ sent.search_headers("Name") +"]\r\n\r\nWrong/Missing password");
        }
        else if(resp_code == "18")
        {
            this->send_pm(botAdmin, "Failed joining ["+ sent.search_headers("Name") +"]\r\n\r\nBanned");
        }
        else
        {
            this->send_pm(botAdmin, "Joined [" + sent.search_headers("Name") + "]");
        }
    }

    //leave group
    if(resp_what == "4")
    {
        if(resp_code == "2")
        {
            this->send_pm(botAdmin, "Failed leaving "+ sent.search_headers("group-id")+"\r\n\r\nNot in group?");
        }
        else if(resp_code == "18")
        {
            this->send_pm(botAdmin, "Failed leaving "+ sent.search_headers("group-id") +"\r\n\r\nBanned");
        }
        else
        {
            this->send_pm(botAdmin, "Left Group "+ sent.search_headers("group-id"));
        }
    }

    //admin actions
    if(resp_what == "19")
    {
        if(resp_code == "13")
        {
            this->send_pm(botAdmin, "Admin action failed\r\n\r\nGroup: "+sent.search_headers("group-id")+"\r\nAction: "+sent.search_headers("Action")+"\r\nTarget: "+sent.search_headers("target-id"));
        }
    }

}

////////////////////////////////////////////////////////////
/* Everything Below This Segmet should remain un touched */
//////////////////////////////////////////////////////////
//base group functions
void 	baseClient::group_join(string groupName, string password)	{ palGroup->group_join(groupName, password); }
void	baseClient::group_part(string groupID)						{ palGroup->group_part(groupID); }

//base message handlers
//void 	baseClient::send_message(string group, string message)		{ if(canTalk) palMesg->send_message(TARGET_GROUP, group, message); }

void 	baseClient::send_message(string group, string message)
{
    messagesSent++;
    palMesg->send_message(TARGET_GROUP, group, message);
}

void 	baseClient::send_pm(string id, string message) 				{ palMesg->send_message(TARGET_PM, id, message); }

void 	baseClient::send_image(string group, string image)		    { palMesg->send_image(TARGET_GROUP, group, image); }
void 	baseClient::send_image_pm(string user, string image)		{ palMesg->send_image(TARGET_PM, user, image); }

//function to send test packet
//packet can be edited in palringoPacket.cpp under ::debug
void    baseClient::send_debug(string to)                           { palMesg->send_debug(TARGET_GROUP, to); }

//base admin functions
void	baseClient::admin_admin  (string groupID, string userID)	{ palGroup->admin(ACTION_ADMIN, groupID, userID); }
void	baseClient::admin_mod    (string groupID, string userID)	{ palGroup->admin(ACTION_MOD, groupID, userID); }
void	baseClient::admin_silence(string groupID, string userID)	{ palGroup->admin(ACTION_SILENCE, groupID, userID); }
void	baseClient::admin_reset  (string groupID, string userID)	{ palGroup->admin(ACTION_RESET, groupID, userID); }
void	baseClient::admin_kick   (string groupID, string userID)	{ palGroup->admin(ACTION_KICK, groupID, userID); }
void	baseClient::admin_ban    (string groupID, string userID)	{ palGroup->admin(ACTION_BAN, groupID, userID); }

//sets pointers
void	baseClient::set_palMesg(palringoMessage *mesg)				{ palMesg = mesg; }
void	baseClient::set_palGroup(palringoGroup *group)				{ palGroup = group; }
void	baseClient::set_palContact(palringoContact *contact)		{ palContact = contact; }

//gets pointers
palringoContact *baseClient::get_palContact()                      { return palContact; }

//public functions to get variables
string 	baseClient::get_Username() 									{ return this->username; }
string 	baseClient::get_Password() 									{ return this->password; }

//various usefull functions

//we start counting at 1 to make out lives eaiser, since vectors.size starts at 1
string baseClient::messagePatcher(vector<string> message, string patch, int start)
{
	string output = "";
	int	   blocks = message.size();

	if(blocks < start)
	{
		for(int i=0; i<blocks; i++)
		{
			output.append(message[i]);
			if(i != blocks-1)
				output.append(patch);
		}
	}
	else if(blocks > start )
	{
		for(int i=start-1; i<blocks; i++)
		{
			output.append(message[i]);
			if(i != blocks-1)
				output.append(patch);
		}

	}
	else if(blocks == start)
	{
		output.append(message[start-1]);
	}
	else
	{
		output.append("null");
	}

	//cout << "Message patcher: " << output << endl;

	return output;
}

//New security functions
bool baseClient::isMod(string userid)
{
    if(botMods.find(userid) != botMods.end())
        return true;
    else
        return false;
}

string baseClient::searchMod(string modname)
{
    for(std::map<string, botMod>::iterator i = botMods.begin(); i != botMods.end(); ++i)
    {
        if(i->second.name == modname)
            return i->second.id;
    }

    return "";
}

bool baseClient::isPest(string userid)
{
    int listSize = botPests.size();

    if(listSize > 0)
    {
        for(int i=0; i<listSize; i++)
        {
            if(botPests[i] == userid)
            {
                return true;
            }
        }
    }

    return false;
}

bool baseClient::shouldMute(string userid)
{
    int listSize = muteList.size();

    if(listSize > 0)
    {
        for(int i=0; i<listSize; i++)
        {
            if(muteList[i] == userid)
            {
                return true;
            }
        }
    }

    return false;
}

bool baseClient::shouldBan(string userid)
{
    int listSize = banList.size();

    if(listSize > 0)
    {
        for(int i=0; i<listSize; i++)
        {
            if(banList[i] == userid)
            {
                return true;
            }
        }
    }

    return false;
}

void baseClient::reloadIni(string group)
{
    INIReader reader = INIReader(this->iniFile);
    string postBuffer = "INILoader Details"; //so we can stop posting multiple messages

    //change command and namespace
    string botName  = reader.Get("SETTINGS", "botName", "UNKNOWN");
    string nameSpace= reader.Get("SETTINGS", "nameSpace", "UNKNOWN");
    string cmdAdmin = reader.Get("SETTINGS", "cmdAdmin", "#");
    string cmdBase  = reader.Get("SETTINGS", "cmdUser", "/");

    if(botName != this->botName && botName != "UNKNOWN")
    {
        this->botName = botName;
        postBuffer += "\r\nUpdated BotName: "+this->botName;
    }

    if(nameSpace != this->nameSpace)
    {
        if(nameSpace == "UNKNOWN")
        {
            //removed
            postBuffer += "\r\nRemoved namespace";

            this->nameSpace  = "UNKNOWN";
            this->patchStart = 2;
            nameSpace = "";
        }
        else
        {
            //updated
            postBuffer += "\r\nUpdated namespace: "+nameSpace;

            this->nameSpace  = nameSpace;
            this->patchStart = 3;
            nameSpace += " ";

        }
    }
    else
    {
        nameSpace += " ";
    }

    if(cmdAdmin+this->nameSpace+" " != this->cmdAdmin)
    {
        if(this->nameSpace != "UNKNOWN")
        {
            this->cmdAdmin = cmdAdmin+nameSpace;
            postBuffer += "\r\nUpdated Admin Command: "+this->cmdAdmin;
        }
        else
        {
            this->cmdAdmin = cmdAdmin;
        }
    }

    if(cmdBase+this->nameSpace+" " != this->cmdBase)
    {
        if(this->nameSpace != "UNKNOWN")
        {
            this->cmdBase = cmdBase+nameSpace;
            postBuffer += "\r\nUpdated User Command: "+this->cmdBase;
        }
        else
        {
            this->cmdBase = cmdBase;
        }
    }

    /////Mod Functions
    botMods.clear(); //reset the list so we can make changes
    string modNumbersInput = reader.Get("MODS", "modAccounts", "null");
    string modNamesInput = reader.Get("MODS", "modNames", "null");
    vector<string> modNumbers;
    vector<string> modNames;

    if(modNumbersInput != "null")
        modNumbers = engine.splitStr(modNumbersInput, '|');

    if(modNamesInput != "null")
        modNames = engine.splitStr(modNamesInput, '|');

    if((modNames.size() == modNumbers.size()))
    {
        int modCount = modNumbers.size();
        for(int i=0; i<modCount; i++)
        {
            botMod temp;
            temp.id = modNumbers[i];
            temp.name = modNames[i];

            this->botMods[modNumbers[i]] = temp;
        }

        if(group != "")
            postBuffer += "\r\nMods: "+modNumbersInput;
    }
    else
        postBuffer += "\r\nNo Mods Found";

    /////Pest Functions
    botPests.clear(); //reset the list so we can make changes
    string pestInput = reader.Get("SECURITY", "ignore", "null");

    if(pestInput != "null" and pestInput != "")
    {
        botPests = engine.splitStr(pestInput, '|');

        if(group != "")
            postBuffer += "\r\nPests: "+pestInput;
    }
    else
        postBuffer += "\r\nNo pests found";

    /////Mute Functions
    muteList.clear(); //reset the list so we can make changes
    string muteInput = reader.Get("SECURITY", "autoMute", "null");

    if(muteInput != "null" and muteInput != "")
    {
        muteList = engine.splitStr(muteInput, '|');

        if(group != "")
            postBuffer += "\r\nMute List: "+muteInput;
    }
    else
        postBuffer += "\r\nNo Mutes Found";

    /////Ban Functions
    banList.clear(); //reset the list so we can make changes
    string banInput = reader.Get("SECURITY", "autoBan", "null");

    if(banInput != "null" and banInput != "")
    {
        banList = engine.splitStr(banInput, '|');

        if(group != "")
            postBuffer += "\r\nBan List: "+banInput;
    }
    else
        postBuffer += "\r\nNo Bans Found";

    //send message buffer
    if(group != "")
        this->send_message(group, postBuffer);
}
