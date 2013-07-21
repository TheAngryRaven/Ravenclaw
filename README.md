# Creator
Raven (ID: 25909767 | 512388) 

This project was originally hosted on bitbucket, the code works, but the copy i used here might be a bit off. I'll make sure it works 100% soon.
***    
## Licenses
RavenClaw: [GNU General Public License, version](http://opensource.org/licenses/GPL-3.0)

Cryptopp: [Boost Software License](http://www.boost.org/users/license.html)

IniReader: [BSD 3-Clause License](http://opensource.org/licenses/BSD-3-Clause)

***    
## Description

A C++ bot base for palringo. Designed for **Windows** It has a handful of basic chat commands and core features. 

A library called crypto++ is required for the bot to function, you can find it in the downloads section.  

Bot is now staying publicly open source, because I feel like it. Be aware it is missing a couple of key features, so start your own fork and help me out!

####Missing Core Feature
* Sub Profile Decryption
>This is the packet sent from palringo telling us things like **display names** and **users status messages** along with **users in a group**.

***
##VERY Recent Changelog

Lots of things have been added to the bot, here is a basic changelog!

* Added a "namespace" feature that pal requires, such as **#rc help** instead of **#help** (but it's optional)
* I updated the main packet decryptor, and it works SOO much better
* I have completed the **groupUpdate** decryptor. You can see it in effect in **baseClient::group_update**
* Updated security functions to ban/mute when a user joins the group
* I have renamed **baseClient::parseCommands** to **baseClient::parse_groupMessage**
* I have fixed a couple little minor bugs, like how incomming messages are display in console
* Added dynamic message IDs on outbound packets so we can actually use response codes in the future
* Added new fields to the INI one required two are optional (more discussed in program notes)
* Added **#info** to show very basic bot statistics
* Updated **#mute** to only effect bot auto responses so commands still work
* Added basic definitions.h to be used to help with various response codes and such, I will expand this to a full class
* Reorganized some of the code in **baseClient::parse_groupMessage** just to be cleaner
* Added section in the INI for the bots userID this is temporary until I can complete the sub profile code

Features that are new but not BRAND NEW

* simply run the command **#reload** to have the bot reload lists out of the ini
* Bot can now have mods, which are added in the ini file
* Mods have their own set of admin commands as defined in **baseClient::parse_groupMessage** and even get to set away messages
* Bot now has an auto ban/mute list which applies to the users when they post or when they join a group
* Also added an ignore list, bot simply ignores ANY input but those users
***
## Compile Help

####Enviroment
>I use [**Code::Blocks**](http://www.codeblocks.org/) with the version of mingw that comes with it

####Compiling CryptoPP
>In the downloads section of the repo you can find a copy of cryptopp I have slightly tweaked to compile with mingw using various internet sources.

>I've included a precompiled version of cryptopp which you can use, simply open up the folder where **Code::Blocks** is installed, typically **C:\Program Files\CodeBlocks\\**, now inside of that folder you will see a folder called **Mingw**, simply copy the **include** and **lib** folder from the archive I've supplied in the downloads section to that folder, it will ask if you want to merger or copy over click yes, you will not be overwritting anything.  
**In Theory** you do not need to recompile cryptopp, if you do here are the basic instructions. 

>* Setup the mingw/bin/ folder to be part of your system path [How To](http://www.computerhope.com/issues/ch000549.htm)
* Copy the **cryptopp** folder from the root of the archive somewhere to your computer
* **DELETE** the **include** and **lib** folders
* Open the folder in an instance of the windows command prompt
* Run the command `mingw32-make` and wait for it to compile you *shouldn't* receive any errors (it takes a while)
* Run the command (can copy and paste) `mkdir include&mkdir include\cryptopp&mkdir lib` to create the folders to stick the binaries in
* Run the command `copy *.h include\cryptopp&copy *.a lib` this copies the headers needed for **Code::Blocks** and the libary to link against
* Now as directed before copy the *include* and *lib* folders to the mingw path in your **Code::Blocks** install path

####Setting up:
>Before you can just simply compile the bot, you need to set a few things up.  
In **Code::Blocks** go to **settings->compiler** now click on the **linker settings** tab and add both **libws2_32.a** and **libcryptopp.a** from **codeblocks\mingw\lib**

>Now in theory, you are ready to build the bot, have fun!

***
##Program Notes:
####Setting login details and such
>The bot can now load details from an INI file, apon starting the bot a basic INI **that you need to edit yourself** (any text editor will do) will be generated called **example.ini** if you do not supply one in the commandline. Along with a super basic **runBot.bat** batch script, which simply contains `ravenclaw.exe exampli.ini`.  

>See the below example for the input the ini excepts.

>**Note**: when HTTPS is enabled, it connects to palringo's secure server, weither or not the data is encrypted is unknown to me, as I have not taken the time to actually go through the packets and such.
    
>Namespace and HTTPS are optional, when nameSpace is removed commands become **#help** instead of **#rc help**

    [LOGON]
    email=youremailhere@gmail.com
    password=yourpassword
	botId=25804994 *NEW
	HTTPS=true	*NEW

    [SETTINGS]
    adminId=yourID
	adminName=ravey
	botName=ravenclaw
	cmdAdmin=#
	cmdUser=/
	nameSpace=rc *NEW
	
	[MODS]
	modAccounts=17779473|2937702
	modNames=nom|andrei

	[SECURITY]
	#first two are welcome/autopost bots
	ignore=15145815|10324473|18089645
	autoBan=18089645
	autoMute=18089645

####Starting the bot
>Simply run a command in the cmd prompt along the lines of

>`ravenclaw.exe example.ini`

>The bot will load the required settings from example.ini, logon and start listening.

####Misc.cpp
>Inside of misc.cpp (usually refered to as engine) I have created some basic and useful features that can be used project wide. The main reason I did this, was to create a "debug printline" which is toggled with debug in misc.cpp, more core features will be added to this function as time progresses. Even has a super simple int to string function `misc::i2s(int number)`

***
###Libraries Used
####libws2_32.a
>For the base socket connector  
Included with most IDEs, its a basic windows lib just link against it in the project.

####libcryptopp.a ver5.61
>For Salsa20/MD5/Hex methods  
>Taken from [Cryptopp 5.61](http://www.cryptopp.com/)

####INIReader version 27
>Super basic ini parser under the BSD license, more of an include than a library.  
The source files are included in the project and require no action from you, but you can find the originals here [Google Code inih](http://code.google.com/p/inih/) if you care to have them.

***
## Core Features
>Many of these features can be removed if desired, this is just a list of everything you get when you download the source.

#### Basic chat listening
>Can listen for keywords in the chat and respond, and do various actions.

>Currently can listen for **phrases** and **keywords** anywhere in a message, along with seeing **#hashtags** at the end of a message.
		
####PM Responses
>When the bot receives a pm, it PMs the bot owner with the message, then the bot owner can reply back using the bot to the original sender. But can be modified for listening to PMs like it listens to chats.

>Now has a dedicated function for parsing private message in **baseClient::parse_pm()**.
	
####Image Sending
>You can now send images inside of `baseClient::` and it's super simple! Second simply call `this->send_image("groupID", "filepath\test.jpg");` or `this->send_image_pm("userID", "filepath\test.jpg");` and away the image goes.

####Full admin functions
>If given elevated premissions can mod/mute/ban etc. 

####Admin triggers
>`baseClient::group_admin()` gets triggered when any admin function is used in a group.
		
####Group join/leave trigger - *NEW AND OUT OF BETA*
>`baseClient::group_update()` gets triggered when anyone joins or leaves a group. Even saves basic contact data that we can call back later, this is the beginning of a contact list.
		
####Bot Admin status
>Default status is offline, when the bot sees that the bot-admin has typed a message in a group the bot is in it sets the admins status to online.

####Find Mod/Admins
>Programmatically the bot owner can set keywords where the bot pms them saying they're being looked for.

####Bot Mod status - *NEW*
>Default status is offline, when the bot sees that a bot-mod has typed a message in a group the bot is in it sets the mods status to online.
		
####Bot Mods - *NEW*
>As shown in the example ini above you can now have bot mods, there is a new if loop in **baseClient::parse_commands()** you can even load new mods without restarting the bot, just add them to the ini and run the command `#loadini` and boom, mods/auto lists now updated!

####Auto ban/mute - *NEW AND OUT OF BETA*
>If a user on the ban/mute list talks **OR** joins a group the bot is in, they automatically muted/banned.

####Ignore - *NEW*
>If a user is on the ignore list the bot simply ignores **ANY** post they make. Helpfull for those abusing the bot.

####Server Responses - *NEW/BETA*
>Currently doesnt work, is only triggered in **baseClient::parseResponse()** will soon be able to listen for all sorts of server responses and hopefully advert all errors if they occur. I have now also added a system to proerly send a "dynamic" message id with each message to properly parse these responses.
***
## List of simple chat commands
>All of the featured are simply in `baseClient::parse_commands()`, nothing is hard coded and can be changed without breaking anything. It is mostly here as an example how ive acomplished some neat little tricks. **Do not** use the less/greator than signs, they are simply there to show you the arguments for the function.

>**Example:** If the command is `/google <query>` you call the function with `/google icecream party`.

####Admin Commands
>Only the bot admin can use these commands.

>**\#help**  
Tells you the available commands, this isn't dynamic if you add more functions be sure to add it to the code.

>**\#leave**  
Makes the bot leave the group you posted the message in.

>**\#join [< group name >] < password >***  - *UPDATED*  
Tells the bot to join whatever group name you want, this function has been updated to support passwords.
Using passwords is rather simple, to join a group with no password simply `#join [group name]` to join with a password `#join [group name] password`. Brackets are now **ONLY required** for the group name, if you do not want to supply a password.

>**\#msg < user id > < message >**  
The bot will PM the user with the supplied **USER ID** with the supplied message.

>**\#away < message >**  
Sets the bot admin to away with an **OPTIONAL** message, if *anyone* mentions the admins name in chat, the bot will tell the group if the admin is online or not.

>**\#secure** (also unsecure)  
Default state is **ON** usefull to program various functions like the included *group name checker* found in `baseClient::parseCommands()`, the function checks if someone posted a link to a group and will scilence them.

>**\#mute** (also unmute)  
Default state is **OFF** bot will still listen and respond to commands. But auto responses and such will not trigger.

>**\#check**  
The bot will post a little message letting you know that it's online even if its set to mute.

>**\#test** (new)  
There is a new packet in **palringo::packet** designed to fill up with whatever data you please, and send to the palringo servers. This command is what triggers the sending of the command.

>**\#reload** (new)  
Reloads the bot's ini file to update ignore/mod/auto lists. It also spits out the output of the reload.

>**\#info** (new)  
Displays various data like messages sent/received and so on.

####Admin PM Commands
>Work similar to the regular admin commands except they are triggered when you PM the bot.They are called from `baseClient::parse_pm()`.  

>**\#help**  
Tells you the available PM commands

>**\#msg < user id > < message >**  
The bot will PM the user with the supplied **USER ID** with the supplied message.
 
>**\#leave < group id >**  
Makes the bot leave the requested group, **NOTE:** supply the group **ID** not **NAME**

>**\#join < group name >**  
Makes the bot join the requested group name.

>**\#uptime**  
Sends you a message displaying the bots uptime in a fashion as such `0d 20h 3m 28s`.

####Mod Commands
>Mods are currently programed to use all admin commands except for **Secure, test and reload**. They currently can also use the PM commands.

####User Commands
>Anyone in the group the bot is in can use these commands.

>**/help**  
Tells everyone the available commands.

>**/google < query >**  
Put simply posts a google search url, doesn't post results to chat.

>**/youtube < query >**  
Put simply posts a youtube search url, doesn't post results to chat.

>**/credits**  
Posts a little message to chat telling people who helped me get the bot going.

>**/admin**  
Tells the group who the admin is and if they are online, if they are offline and the admin has set an away message, displays that message.

>**/mods** (new)  
Tells the group who all the assigned mods are and if they are online or not.

>**/uptime**  
Posts how long the bot has been running.

>**/dice < coin,6,8,10,12 or 20 >**  
Very basic random number generator, posts results to group.

>**/website**  
Posts a link to the BitBucket Repo for ravenclaw.
***
###Other Features
>These are simple features that trigger with certain messages.

####Crude AI
>Simply using `botname phrase` can trigger cute little responses from the bot. I've only given ravenclaw a couple "AI" triggers, its just a giant if statement, its super easy to add your own.

>**Example:** saying `ravenclaw how are you?` would trigger the bot to respond `I'm good thanks for asking!`.

####Filters
>In the else statement in `baseClient::parse_commands()` commented with `/* filters */` are various ways to call various types of message *"searching"*. Explaining all the current triggers can make this readme EVEN MORE lengthy so ive decided not to do that, but instead describe the types of triggers I have coded, and a single example. All triggers are simply if statements and it takes seconds to add your own.

>**Phrases**  
if the message is the phrase and ONLY the phrase, the event triggers.  
Example phrase is: `bye guys` triggers the bot to post `/me waves goodbye`

>**Word Filters**  
If a users post contains the word **ANYWHERE** in the message, the event triggers.  
Example: `shit` and some other curse words trigger the bot to call them a potty mouth.

>**Internal Phrase Triggers**  
The example ive put in the code is the `call of duty` trigger, slightly complicated code wise but not hard to wrap your head around, these trigger if the phrase is **ANYWHERE** in the users message.

>**HashTags**  
If someone uses a hashtag phrase **IE:** `#yolo` at the end of a message, this triggers the bot to post a very silly message about the use of yolo.

***
###TODO
####Sub Profile Reading
>this handles all the text usernames and statuses and such.
		
####Linux compatibility
>The entire project is virtually linux ready, the only non linux code is currently in my base connection class. Cryptopp works with zero problems I even ran a few tests.

####Clean up code
>Fuck yea re-did packet parser, and a new packet debug output.
		
####Make the bot scripted
>No one wants to recompile the bot, the goal is once the base is nailed out and polished, impiliment python or another scripting language as a frontend, and keep RavenClaw as a backend, that only needs to be compiled for new features and bugs.