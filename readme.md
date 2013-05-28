# Creator
Raven (ID: 25909767 | 512388)
***	
## Licenses
RavenClaw: [http://opensource.org/licenses/GPL-3.0](http://opensource.org/licenses/GPL-3.0)

Cryptopp: [http://www.boost.org/users/license.html](http://www.boost.org/users/license.html)

IniReader: [http://opensource.org/licenses/BSD-3-Clause](http://opensource.org/licenses/BSD-3-Clause)

***	
## Description

>A C++ bot base for palringo. Designed for **Windows** It has a handful of basic chat commands and core features.

>A library called crypto++ is required for the bot to function, you can find it in the downloads section.

>Bot is now staying publicly open source, because I feel like it. Be aware it is missing a couple of key features, so start your own fork and help me out!

####Missing Core Features
* Sub Profile Decryption
>This is the packet sent from palringo telling us things like **display names** and **users status messages** along with **users in a group**.

* Group Update Parsing
>We have a function that activates on group update, but haven't quite gotten around to figuring out what data is where in the payload, its oddly serialized.
***
## Compile Help

####Enviroment
>I use [**Code::Blocks**](http://www.codeblocks.org/) with the version of mingw that comes with it

####Compiling CryptoPP
>In the downloads section of the repo you can find a copy of cryptopp i have slightly tweaked to compile with mingw using various internet sources.

>I've included a precompiled version of cryptopp which you can use, simply open up the folder where **Code::Blocks** is installed, typically **C:\Program Files\CodeBlocks\\**, now inside of that folder you will see a folder called **Mingw**, simply copy the **include** and **lib** folder from the archive i've supplied in the downloads section to that folder, it will ask if you want to merger or copy over click yes, you will not be overwritting anything.  
**In Theory** you do not need to recompile cryptopp, if you do here are the basic instructions. 

>* Setup the mingw folder to be part of your system path [How To](http://www.computerhope.com/issues/ch000549.htm)
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
>The bot can now load details from an INI file, apon starting the bot a basic INI **that you need to edit yourself** will be generated called **example.ini** if you do not supply one in the commandline. Along with a super basic **runBot.bat** batch script, which is simply *ravenclaw.exe exampli.ini*.

>The bot accepts INI settings for; **Email** - **password** - **bot admin id** - **Bot admin name** *and* **Bot name**. It also features 2 bonus arguments **cmdAdmin**, *and* **cmdUser**. The last two are optional and by default are *#* and */* respectively.

####Starting the bot
>Simply run a command in the cmd prompt along the lines of

>`ravenclaw.exe example.ini`

>The bot will load the required settings from example.ini, logon and start listening.

####Misc.cpp
>Inside of misc.cpp (usually refered to as engine) I have created some basic and useful features that can be used project wide. The main reason I did this, was to create a "debug printline" which is toggled with debug in misc.cpp, more core features will be added to this function as time progresses.

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
#### Basic chat listening
>Can listen for keywords in the chat and respond, and do various actions.

>Currently can listen for **phrases** and **keywords** anywhere in a message, along with seeing **#hashtags** at the end of a message.
		
####PM Responses
>When the bot receives a pm, it PMs the bot owner with the message, then the bot owner can reply back using the bot to the original sender. But can be modified for listening to PMs like it listens to chats.

>Now has a dedicated function for parsing pm commands in **baseClient::parse_pm()** some things still parsed in **baseClient::recv_pm()** though.
	
####Image Sending (new)
>You can now send images inside of **baseClient::** and it's super simple! At the moment it only supports **JPEG** images. Second simply call `this->send_image("groupID", "filepath\test.jpg");` or `this->send_image_pm("userID", "filepath\test.jpg");` and away the image goes.

####Full admin functions
>If given elevated premissions can mod/mute/ban etc etc. 

####Admin triggers
>**baseClient::group_admin()** gets triggered when any admin function is used in a group.
		
####Group join/leave trigger
>**baseClient::group_update()** gets triggered when anyone joins or leaves a group BUT cannot currently tell who or where.
		
####Bot Admin status
>Default status is offline, when the bot sees that the bot-admin has typed a message in a group the bot is in it sets the admins status to online.
		
>Problematically the bot owner can set keywords where the bot pms them saying they're being looked for.
This is not a hard coded feature, just a neat one I like.

***
## List of simple chat commands
>All of the featured are simply in **baseClient::parse_commands()**, nothing is hard coded and can be changed without breaking anything. It is mostly here as an example how ive acomplished some neat little tricks. **Do not** use the brackets, they are simply there to show you the arguments for the function.

>**Example:** If the command is **/google [query]** you call the function with **/google icecream party**.

####Admin Commands
>Only the bot admin can use these commands.

>**\#help**  
Tells you the available commands, this isn't dynamic if you add more functions be sure to add it to the code.

>**\#leave**  
Makes the bot leave the group you posted the message in.

>**\#join *[group name]***  
Tells the bot to join whatever group name you want, currently does not support passwords, thats on the minor todo list.

>**\#msg *[user id]* *[message]***  
The bot will PM the user with the supplied **USER ID** with the supplied message.

>**\#away *[message]***  
Sets the bot admin to away with an **OPTIONAL** message, if *anyone* mentions the admins name in chat, the bot will tell the group if the admin is online or not.

>**\#secure** (also unsecure)  
Default state is **ON** usefull to program various functions like the included *group name checker* found in **baseClient::parseCommands()**, the function checks if someone posted a link to a group and will scilence them.

>**\#mute** (also unmute)  
Default state is **OFF** bot will still do admin commands in code, but will not post messages to the group, useful if someone keeps triggering the *potty mouth** function.

>**\#check**  
The bot will post a little message letting you know that it's online even if its set to mute.

####Admin PM Commands
>Work similar to the regular admin commands except they are triggered when you PM the bot.  

>**\#help**  
Tells you the available PM commands

>**\#msg [user id] [message]***  
The bot will PM the user with the supplied **USER ID** with the supplied message.
 
>**\#leave [group id] ***  
Makes the bot leave the requested group, **NOTE:** supply the group **ID** not **NAME**

>**\#join [group name]***  
Makes the bot join the requested group name.

>**\#uptime***  
Sends you a message displaying the bots uptime.


####User Commands
>Anyone in the group the bot is in can use these commands.

>**/help**  
Tells everyone the available commands.

>**/google [query]**  
Put simply posts a google search url, doesn't post results to chat.

>**/youtube [query]**  
Put simply posts a youtube search url, doesn't post results to chat.

>**/credits**  
Posts a little message to chat telling people who helped me get the bot going.

>**/admin**  
Tells the group who the admin is and if they are online, if they are offline and the admin has set an away message, displays that message.

>**/uptime**  
Posts how long the bot has been running.

>**/dice [coin,6,8,10,12 or 20]**  
Very basic random number generator, posts results to group.

>**/website**  
Posts a link to the BitBucket Repo for ravenclaw.

####Crude AI
>Simply using **botname phrase** can trigger cute little responses from the bot. I've only given ravenclaw a couple "AI" triggers, its just a giant if statement, its super easy to add your own.

>**Example:** saying **ravenclaw how are you?** would trigger the bot to respond **I'm good thanks for asking!**.

####Filters
>In the else statement in **baseClient::parse_commands()** commented with `/* filters */` are various ways to call various types of message *"searching"*. Explaining all the current triggers can make this readme EVEN MORE lengthy so ive decided not to do that, but instead describe the types of triggers I have coded, and a single example. All triggers are simply if statements and it takes seconds to add your own.

>**Phrases**  
if the message is the phrase and ONLY the phrase, the event triggers.  
Example phrase is: **bye guys** triggers the bot to post **/me waves goodbye**

>**Word Filters**  
If a users post contains the word **ANYWHERE** in the message, the event triggers.  
Example: **shit** and some other curse words trigger the bot to call them a potty mouth.

>**Internal Phrase Triggers**  
The example ive put in the code is the **call of duty** trigger, slightly complicated code wise but not hard to wrap your head around, these trigger if the phrase is **ANYWHERE** in the users message.

>**HashTags**  
If someone uses a hashtag phrase **IE: #yolo** at the end of a message, this triggers the bot to post a very silly message about the use of yolo.

***
###TODO
####Fix group join / leave events
>Figure out how to decode the packet telling me information like what group and who left/joined.
	
####Sub Profile Reading
>this handles all the text usernames and statuses and such.
		
####Clean up code
>its currently very messy and somewhat unorganized.

####Figure out if its possible to recreate the functionality of youtube bot
>No idea if its possible, havent looked into it, would like to see if its possible.
		
####Make the bot scripted
>No one wants to recompile the bot, the goal is once the base is nailed out and polished, impiliment python or another scripting language as a frontend, and keep RavenClaw as a backend, that only needs to be compiled for new features and bugs.