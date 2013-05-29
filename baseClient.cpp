/*
 * Software created by:
 * Michael Champagne
 * Raven (ID: 22885233 | 512388)

 * Licensed with GNU lpgl v3.0
 * Found in gnu-v3.txt or available online here
 * http://www.gnu.org/licenses/lgpl.txt
 */

#include "baseClient.h"

baseClient::baseClient(string username, string password, string botAdmin, string adminName, string botName, string cmdAdmin, string cmdUser)
{
    //Login Details | No more compile time username and password
	this->username 	= username;
	this->password 	= password;

	this->botAdmin 	= botAdmin;     //the ID of the user who owns the bot
	this->botName	= botName;	    //Set the bots name to give it personality
	this->adminName	= adminName;    //used for printing the admins name in messages

	//these set the way you call commands
	//ie #leave or /help
	this->cmdAdmin 	= cmdAdmin;
	this->cmdBase	= cmdUser;

    //Feature is currently depreciated
    //A way to save the group number for special bot commands
	//controlGroup      = "[Group ID]"; //palringo
	//controlGroupName  = "[Group ID]"; //palringo

	//various "system" variables
	canTalk     = true;     //if the bot is muted or not
	adminOnline = false;    //if admin is online or not
	startTime   = time(NULL);//sets the time the bot initializes
	security    = true;    //activates things like the group parser
}

//triggered when a message is received
void baseClient::recv_message(string group, string user, string message)
{
    this->parse_commands(group, user, engine.splitStr(message, ' '));
}

//triggered when a PM is received
void baseClient::recv_pm(string name, string user, string message)
{
    this->parse_pm(name, user, engine.splitStr(message,' '));
}

//triggered when anyone joins or leaves a group
void baseClient::group_update()
{
    //TODO: fully furnish, doesnt work at all except for the fact it gets triggered
    engine.pl("palClient-> someone joined or left a group");
}

//triggered when anyone commits an admin action
void baseClient::group_admin(string group, string admin, string user, string action)
{
    //TODO: works, just need more details coded

    string op       = "1";
    string mod      = "2";
    string silence  = "8";
    string reset    = "0";
    string kick     = "16";
    string ban      = "4";

    if(action == op)
        this->send_message(group, "Congrats!\r\nJust dont power abuse");
    else if(action == mod)
        this->send_message(group, "Aww thats cute\r\nYou think you have power");
    else if(action == silence)
        this->send_message(group, "I love duct tape!");
    else if(action == reset)
        this->send_message(group, "Yay they reset you :3");
    else if(action == kick)
        this->send_message(group, "Get told");
    else if(action == ban)
        this->send_message(group, "Here comes the buthurt");
}

//ive left in some of the basic bot features im writting for my personal bot to give you a feel of how i do things
void baseClient::parse_commands(string group, string user, vector<string> data)
{
	int		blocks		= data.size();
	string	cmd			= data[0];
	string  mesg        = this->messagePatcher(data);

	if(user == botAdmin && adminOnline == false)
	{
		//sets the bot admins status to being back online
		adminOnline = true;
		adminMessage = "";
		this->send_message(group, adminName+" is back online!");
	}

    //little security features like muting people who post group links
    if(security == true && user != botAdmin)
    {
        //primitive auto ban system, once group update is more furnished this will be moved into there
        if(user == "18089645")
        {
            //currently the user "crimson" is the only target, they spoiled the fun by getting the bot banned
            this->admin_ban(group, user);
        }

        //checks if someone posted a group link thats not the welcome or autopost bot
        if((user != "15145815" && user != "10324473") && mesg.find("[") != std::string::npos && mesg.find("]") != std::string::npos)
        {
            this->send_message(group, "Don't post group links!");

            //finds the group they posted

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

    if(user == botAdmin)
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
            if(blocks >= 2)
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
                        this->group_join(groupName, password);
                    }
                }
            }
            else
            {
                this->send_message(group, cmdAdmin+"join [<group name>] <password>");
            }
        }
        else if(cmd == cmdAdmin+"msg")
        {
            if(blocks > 2)
            {
                //generate message
                string buff = this->messagePatcher(data, " ", 2);

                //send the buffer
                this->send_pm(data[1], buff);
                this->send_message(group, "Message Sent!");
            }
            else
            {
                this->send_message(group, cmdAdmin+"msg <user ID> <message>");
            }
        }
        else if(cmd == cmdAdmin+"away")
        {
            /* Allows the bot admin to go afk, and the bot will know your not online, and leaves a message showing your not online */
            if(blocks > 1)
            {
                adminMessage	= this->messagePatcher(data, " ");
                adminOnline		= false;
                this->send_message(group, "Bye " + adminName + "!\r\nAnd don't worry I saved the message");
            }
            else
            {
                adminOnline		= false;
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
            this->send_message(group, "Sending developer packet");
            this->send_debug(group);
        }
        else if(cmd == cmdAdmin+"help")
        {
             this->send_message(group, 	"Admin Help\r\n"+
                                        cmdAdmin+"check\r\n"+
                                        cmdAdmin+"leave\r\n"+
                                        cmdAdmin+"join [<group name>] <password>\r\n"+
                                        cmdAdmin+"msg <user id> <message>\r\n"+
                                        cmdAdmin+"away <message>\r\n"+
                                        cmdAdmin+"secure (also unsecure)\r\n"+
                                        cmdAdmin+"mute (also unmute)"
                                        );
        }
    }
	else if(cmd == cmdBase+"admin")
    {
        string output = "The bot owner is "+adminName+" Userid: "+botAdmin;
        //this->send_message(group, "The bot owner is "+adminName+" Userid: "+botAdmin);

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
		this->send_message(group, 	"User Help\r\n"+
                                    cmdBase+"google <query>\r\n"+
									cmdBase+"youtube <query>\r\n"+
									cmdBase+"credits\r\n"+
									cmdBase+"admin\r\n"+
									cmdBase+"uptime\r\n"+
									cmdBase+"dice <coin,6,8,10,12 or 20>\r\n"+
									cmdBase+"website");
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
		if(blocks > 1)
		{
			string search = "Here you go\r\nhttp://www.google.com/search?q=";
			search.append(this->messagePatcher(data, "+"));
			this->send_message(group, search);
		}
		else
		{
			this->send_message(group, "Sorry thats not how you use this command\r\n"+cmdBase+"google <search query>");
		}
	}
	else if(cmd == cmdBase+"youtube")
	{
		if(blocks > 1)
		{
			string search = "Videos are fun!\r\nhttp://www.youtube.com/results?search_query=";
			search.append(this->messagePatcher(data, "+"));
			this->send_message(group, search);
		}
		else
		{
			this->send_message(group, "Sorry hun thats not how you use this command\r\n"+cmdBase+"youtube <search query>");
		}
	}
	else if(cmd == cmdBase+"dice")
    {
        if(blocks == 2)
		{
			srand (time(NULL));
			int dieRoll = 0;
			string coin = "";


			if(data[1] == "coin")
			{
			    dieRoll = rand() % 50 + 1;

			    if(dieRoll < 25)
                    coin = "heads";
                else
                    coin = "tails";
			}
			else if(data[1] == "6" || data[1] == "d6")
                dieRoll = rand() % 6 + 1;
			else if(data[1] == "8" || data[1] == "d8")
                dieRoll = rand() % 8 + 1;
			else if(data[1] == "10" || data[1] == "d10")
                dieRoll = rand() % 10 + 1;
			else if(data[1] == "12" || data[1] == "d12")
                dieRoll = rand() % 12 + 1;
			else if(data[1] == "20" || data[1] == "d20")
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
	else if(cmd == botName) /* "talk to the bot" */
	{
		if(blocks >= 2)
		{
			string aiCommand = this->messagePatcher(data, " ");

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

	else /* filters */
	{
		if(blocks >= 1) /* phrase filters */
		{
			//generates our phrase with our magical messagePatcher <3
			string phrase = this->messagePatcher(data, " ", 0);

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
					else if(data[i] == "u" || data[i] == "c" || data[i] == "r" || data[i] == "y" || data[i] == "o")
					{
						this->send_message(group, "Use words not letters\r\nDon't be lazy!");
						//this->admin_silence(group, user);
						break;
					}
					else if(data[i] == "ass" || data[i] == "bitch" || data[i] == "fuck" || data[i] == "shit" || data[i] == "damn" || data[i] == "hell" || data[i] == "cunt")
					{
						this->send_message(group, "Potty Mouth!!!");
						//this->admin_silence(group, user);
						break;
					}
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
void baseClient::parse_pm(string name, string user, vector<string> data)
{
	int		blocks		= data.size();
	string	cmd			= data[0];
	string  mesg        = this->messagePatcher(data);

    if(user == botAdmin)
    {
        if(cmd == cmdAdmin+"msg")
        {
            if(blocks > 2)
            {
                //generate message
                string buff = this->messagePatcher(data, " ", 2);

                //send the buffer
                this->send_pm(data[1], buff);
            }
            else
            {
                this->send_pm(user, "#msg <user ID> <message>");
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
            if(blocks == 2)
            {
                this->group_part(data[1]);
            }
            else
            {
                this->send_pm(user, cmdAdmin+"leave <group id>");
            }
        }
        if(cmd == cmdAdmin+"join")
        {
            if(blocks >= 2)
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
                        this->group_join(groupName, password);
                    }
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
                                    cmdAdmin+"join <group name>\r\n"+
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

		this->send_pm(botAdmin, buffer);
		buffer = "";
		buffer.append("#msg ");
		buffer.append(user);
		buffer.append(" ");
		this->send_pm(botAdmin, buffer);
    }
}

////////////////////////////////////////////////////////////
/* Everything Below This Segmet should remain un touched */
//////////////////////////////////////////////////////////
//base group functions
void 	baseClient::group_join(string groupName, string password)	{ palGroup->group_join(groupName, password); }
void	baseClient::group_part(string groupID)						{ palGroup->group_part(groupID); }

//base message handlers
void 	baseClient::send_message(string group, string message)		{ if(canTalk) palMesg->send_message("1", group, message); }
void 	baseClient::send_pm(string id, string message) 				{ palMesg->send_message("0", id, message); }

void 	baseClient::send_image(string group, string image)		    { palMesg->send_image("1", group, image); }
void 	baseClient::send_image_pm(string user, string image)		{ palMesg->send_image("0", user, image); }

//function to send test packet
//packet can be edited in palringoPacket.cpp under ::debug
void    baseClient::send_debug(string to)                           { palMesg->send_debug("1", to); }

//base admin functions
void	baseClient::admin_admin  (string groupID, string userID)	{ palGroup->admin("1", groupID, userID); }
void	baseClient::admin_mod    (string groupID, string userID)	{ palGroup->admin("2", groupID, userID); }
void	baseClient::admin_silence(string groupID, string userID)	{ palGroup->admin("8", groupID, userID); }
void	baseClient::admin_reset  (string groupID, string userID)	{ palGroup->admin("0", groupID, userID); }
void	baseClient::admin_kick   (string groupID, string userID)	{ palGroup->admin("16", groupID, userID); }
void	baseClient::admin_ban    (string groupID, string userID)	{ palGroup->admin("4", groupID, userID); }

//sets pointers
void	baseClient::set_palMesg(palringoMessage *mesg)				{ palMesg = mesg; }
void	baseClient::set_palGroup(palringoGroup *group)				{ palGroup = group; }

//public functions to get variables
string 	baseClient::get_Username() 									{ return this->username; }
string 	baseClient::get_Password() 									{ return this->password; }

//various usefull functions
string baseClient::messagePatcher(vector<string> message, string patch, int start)
{
	string output = "";
	int	   blocks = message.size();

	if(blocks > start+1)
	{
		for(int i=start; i<blocks; i++)
		{
			output.append(message[i]);
			if(i != blocks-1)
				output.append(patch);
		}
	}
	else if(blocks == 2)
	{
		output.append(message[start]);
	}
	else if(blocks == start)
    {
        output.append(message[0]);
    }
	else if(blocks == start+1)
    {
        output.append(message[start]);
    }
	else
	{
		output.append("null");
	}

	return output;
}

