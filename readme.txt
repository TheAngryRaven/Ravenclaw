Creator:
	Raven (ID: 22885233 | 512388)
	
Licenses:
	RavenClaw: http://opensource.org/licenses/GPL-3.0
	Cryptopp: http://www.boost.org/users/license.html
	
Description:
	A C++ bot base for palringo. It has a handfull of many basic operations.
	A library called crypto++ is required for the bot to function, until I feel that I have polished the bot well enough, you must find and compile this yourself. Until I feel it is polished enough for a true "release" I dont want the bot to be "noob" friendly.
	
	If anyone knows C++ to a well enough extent that they feel they can help, I would be glad to help them get the library up and running, so that they can help with the project. I don't ask that you know much, but if you have even a primitive understanding of c++ to the point you can handle pointers (I am far from an expert myself) I would be greatfull of your help.
	
	This isn't as feature rich as some of the C# bots, but uses C++ which makes it much more challenging and fun to program for. Plus since its c++ you could integrate all kinds of cool scripting languages like python.
	
	Right now this is more or less a bot base, once polished, I will also release the source for a bot that does other things besides core features as a fork.
	
Program Notes: 
	Setting login details and such,
		Not quite an ini file but close enough for government work
		
		The bot now accepts commandline arguments for, Email - password - bot admin id - Bot admin name and Bot name. It also features 2 bonus arguments cmdAdmin, and cmdUser. The last two are optional and by default are # and / respectively.
		
		It is good practice to wrap all your arguments in quotes ie: "example@email.com" sometimes special characters trigger things in the command line, this is mostly true using special chars for arguments, OR if you are using spaces in any of the arguments, Test Name will break arguments, use "Test Name" instead.
		
	Example command line arguments
	You can just create a batch file or a shortcut so you dont have to always type them
	
	`ravenclaw.exe example@email.com 12345 22885233  Ravey crimson "$" "&"`

	>Now all admin commands are $command rather than #command and user commands are &help rather than /help
			
	Misc.cpp
		Inside of misc.cpp (usually refered to as engine) I have created some basic and usefull features that can be used project wide. The main reason I did this, was to create a "debug printline" which is toggled with debug in misc.cpp, more core features will be added to this function later on.

Libraries		
	libws2_32.a
		For the base socket connector

	libcryptopp.a
		For Salsa20/MD5/Hex methods	

Features:
	Basic chat listening, 
		Can listen for keywords in the chat and respond.
		
	PM Responses,
		When the bot receives a pm, it PMs the bot owner with the message, then the bot owner can reply back using the bot to the original sender. But can be modified for listening to PMs like it listens to chats.
		
	Full admin functions,
		If given elevated premissions can mod/mute/ban etc etc. 
		
	Bot Admin status,
		Default status is offline, when the bot sees that the bot admin has typed a message in a group the bot is in it sets the admins status to online.
		
		Programatically the bot owner can set keywords where the bot pms them saything they're being looked for. Usefull for checking if the admin the bot belongs to is online.

TODO:
	Random bot crash,
		I have appeared to have fixed the bug, the bot would go into a "black hole" state when it was GHOSTED, fix that, and fixed how pings are sent. So far no black holes, think I fixed it.
	
	Group join / leave events
		make the bot respond to people leaving and joining.
		
	Image sending,
		I've got the packets needed to send, just need to figure out how to seralize the image.
	
	Sub Profile Reading,
		this handles all the usernames and statuses and such.
		
	Clean up code,
		its currently very messy and somewhat unorganized.
		
	Make the bot scrypted,
		No one wants to recompile the bot, the goal is once the base is nailed out and polished, impiliment python or another scripting language as a frontend, and keep RavenClaw as a backend, that only needs to be compiled for new features and bugs.